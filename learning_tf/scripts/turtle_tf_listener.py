#!/usr/bin/env python  
import roslib
roslib.load_manifest('learning_tf')
import rospy
import math
import tf
import geometry_msgs.msg
import turtlesim.srv
import sys

if __name__ == '__main__':
    rospy.init_node('tf_turtle')

    listener = tf.TransformListener()

    rospy.wait_for_service('spawn')
    spawner = rospy.ServiceProxy('spawn', turtlesim.srv.Spawn)
    spawner(4, 2, 0, 'turtle2')

    turtle_vel = rospy.Publisher('turtle2/cmd_vel', geometry_msgs.msg.Twist,queue_size=1)

    rate = rospy.Rate(10.0)
    while not rospy.is_shutdown():
        try:
            ## get trans & rot of turtle1 from turtole2
            ## case1: lookup the latest transform
            # (trans,rot) = listener.lookupTransform('/turtle2', '/turtle1', rospy.Time(0))
            ## case2: wait & lookup
            # listener.waitForTransform('/turtle2', '/turtle1', rospy.Time(0),rospy.Duration(0.001))
            # (trans,rot) = listener.lookupTransform("/turtle2", "/turtle1", rospy.Time(0))
            ## case3: lookup previous time
            fiveSecAgo=rospy.Time.now()-rospy.Duration(5)
            
            (trans,rot) = listener.lookupTransformFull('/turtle2', fiveSecAgo, '/turtle1', fiveSecAgo, "/world")


        except tf.Exception:
            print "[%s] : %s" % ( rospy.Time.now(), sys.exc_info()[0] )
            continue
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            print "cannot listen"
            continue

        angular = 4 * math.atan2(trans[1], trans[0])
        linear = 0.5 * math.sqrt(trans[0] ** 2 + trans[1] ** 2)
        cmd = geometry_msgs.msg.Twist()
        cmd.linear.x = linear
        cmd.angular.z = angular
        turtle_vel.publish(cmd)

        rate.sleep()
