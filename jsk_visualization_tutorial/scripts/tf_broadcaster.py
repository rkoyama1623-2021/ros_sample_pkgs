#!/usr/bin/env python
# -*-coding:utf-8-*-

import rospy
import tf
import sys


def handle_turtle_pose(frame_id,child_frame_id):
    br = tf.TransformBroadcaster()
    br.sendTransform((0, 0, 0.668),
                     tf.transformations.quaternion_from_euler(0, 0, 0),
                     rospy.Time.now(),
                     child_frame_id, frame_id)
if __name__ == '__main__':
    print "start"
    rospy.init_node('tf_broadcaster')
    r = rospy.Rate(1) # 1hz
    p='p';c='c';
    if len(sys.argv) > 1:
        try:
            p=sys.argv[1]
            c=sys.argv[2]
        except:
            pass
    while not rospy.is_shutdown():
        handle_turtle_pose(p,c)
        r.sleep()
