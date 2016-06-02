# about this package
this is a sample package for ros
first this package is created by follow commands:

```bash
catkin_create_pkg beginner_tutorials std_msgs rospy roscpp
```
If you want to check the first dependancy, you should execute following commands.
```
rospack depends1 beginner_tutorials 
```

# memo
## publish force topoic
```
rostopic pub -r 1 /rhsensor geometry_msgs/WrenchStamped '{header: {frame_id: "hoge"}, wrench: {force: {x: 1, y: 0, z: 0}}}'
```

