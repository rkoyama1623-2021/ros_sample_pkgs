#!/usr/bin/env roseus

(load "package://hrpsys_ros_bridge_tutorials/euslisp/samplerobot-interface.l")
(load "package://jsk_interactive_marker/euslisp/display-robot-state.l")
(setq *robot* (instance samplerobot-robot :init))
(send *robot* :fix-leg-to-coords (make-coords))
(ros::roseus "foo")
(ros::advertise "/robot_state" moveit_msgs::DisplayRobotState)
(ros::publish "/robot_state" (angle-vector-to-display-robot-state *robot* (send (send *robot* :waist_link0_lk) :copy-worldcoords)))
(objects (list *robot*))

(print "(ros::publish \"/robot_state\" (angle-vector-to-display-robot-state *robot* (send (send *robot* :waist_link0_lk) :copy-worldcoords)))")


;; see rsoeus_tutorial/src/robot-publisher.l
