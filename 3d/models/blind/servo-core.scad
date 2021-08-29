use <../lib/2d.scad>
use <../lib/3d.scad>
use <../lib/box.scad>
    w1 = 40.55;
    w2 = 44.9;
    h1 = 5.4;
    h2 = 1.75;
    r = 1;
    l1 = 30;
    side_walls_thickness = 2;
    thick_side_walls_thickness = 2.0;
    bearing_w = 6;
    bearing_fi = 16.2;
    axis_level = 35;
    marigin = 5;
    bearing_distance = 35;
    bearing_wall = 8;
    servo_distance = 35;
    servo_wall = 5;
    servo_w = 20.2;
    servo_h = 41;
    servo_axis_offset = 11;
    servo_mount_depth = 28;
    element_distance = 20;
    screw_width = 5.7;
    screw_height = 2.5;
    screw_mount_width = 10;
    screw_mount_marigin = 2;
    screw_distance = 30;
    screw_fi = 3;
    sqrt3 = 1.732;
    sqrt2 = 1.4142;
    screw_mount_thickness = screw_height+screw_mount_marigin;
    reinforcement_width = 7;
module reinforcement() {
    difference() {
        color ("pink") cube ([reinforcement_width, reinforcement_width, side_walls_thickness]);
        translate([0, 0, -0.1]) color ("blue") rotate([0, 0, 45]) cube ([sqrt2*screw_mount_width, sqrt2*screw_mount_width, side_walls_thickness+2*0.1]);
    }
}
module reinforcement_tower() {
    difference() {
        color ("pink") cube ([reinforcement_width, reinforcement_width, servo_h+h1]);
        translate([0, 0, -0.1]) color ("blue") rotate([0, 0, 45]) cube ([sqrt2*screw_mount_width, sqrt2*screw_mount_width, servo_h+h1+2*0.1]);
    }
}
module servo_wall_screw_mount_female() {
    difference() {
        cube ([w2, screw_mount_width, screw_mount_thickness]);
        translate([screw_width/sqrt3 + (w2-2*screw_width/sqrt3-screw_distance)/2, screw_width/sqrt3 + (screw_mount_width-2*screw_width/sqrt3)/2, -0.1]) union() {
            cylinder (screw_height, screw_width/sqrt3, screw_width/sqrt3, $fn=6);
            translate([screw_distance, 0, -0.1]) cylinder (screw_height, screw_width/sqrt3, screw_width/sqrt3, $fn=6);
        }
        translate([screw_width/sqrt3 + (w2-2*screw_width/sqrt3-screw_distance)/2, screw_width/sqrt3 + (screw_mount_width-2*screw_width/sqrt3)/2, -0.1]) cylinder(screw_mount_width, screw_fi/2, screw_fi/2, $fn=100);
        translate([screw_width/sqrt3 + (w2-2*screw_width/sqrt3-screw_distance)/2 + screw_distance, screw_width/sqrt3 + (screw_mount_width-2*screw_width/sqrt3)/2, -0.1]) cylinder(screw_mount_width, screw_fi/2, screw_fi/2, $fn=100);
    }
}
module servo_wall_screw_mount_male() {
    difference() {
        cube ([w2, screw_mount_width, screw_mount_thickness]);
        translate([screw_width/sqrt3 + (w2-2*screw_width/sqrt3-screw_distance)/2, screw_width/sqrt3 + (screw_mount_width-2*screw_width/sqrt3)/2, -0.1]) cylinder(screw_mount_width, screw_fi/2, screw_fi/2, $fn=100);
        translate([screw_width/sqrt3 + (w2-2*screw_width/sqrt3-screw_distance)/2 + screw_distance, screw_width/sqrt3 + (screw_mount_width-2*screw_width/sqrt3)/2, -0.1]) cylinder(screw_mount_width, screw_fi/2, screw_fi/2, $fn=100);
    }
}
module handle() {
    // main element
    difference() {
        union() {          
            // base
            translate([0, -30, 0]) color("gray") union() {
                // long chassis !!!
                translate([(w2 - w1)/2, 0, 0]) color("blue") cube ([w1, l1+bearing_distance+servo_distance, h1]);
                // long chassis !!!
                translate([0, 0, h1]) color("pink") rounded_cube (w2, l1+bearing_distance+servo_distance, h2, r);
                s = 5;
                translate([0, r + s, h1+h2]) cube ([2, l1-r-s, 4]);
                translate([w2-2, r + s, h1+h2]) cube ([2, l1-r-s, 4]);
            }
            difference() {
                union() {
                    // front wall - first bearing
                    difference() {
                        cube ([w2, bearing_wall, axis_level+bearing_fi/2+marigin]);
                        translate ([w2/2, bearing_wall-bearing_w, axis_level]) rotate([-90, 0, 0]) cylinder(bearing_w+1, bearing_fi/2, bearing_fi/2, $fn=100);
                    }
                    // second wall - second bearing
                    difference() {
                        translate ([0, bearing_distance, 0]) cube ([w2, bearing_wall, axis_level+bearing_fi/2+marigin]);
                        translate ([w2/2, bearing_distance-0.1, axis_level]) rotate([-90, 0, 0]) cylinder(bearing_w, bearing_fi/2, bearing_fi/2, $fn=100);
                    }
                }
                // axis
                translate([w2/2, -1, axis_level]) color("pink") rotate([-90, 0, 0]) cylinder(bearing_distance+bearing_wall+side_walls_thickness, 8.2/2, 8.2/2, $fn=100);
            }
            // side walls
            cube ([side_walls_thickness, bearing_distance+servo_distance, axis_level]);
            translate([w2-2, 0, 0]) cube ([side_walls_thickness, bearing_distance+servo_distance, axis_level]);
            translate ([-screw_mount_width, bearing_distance + servo_distance - screw_mount_thickness, 0]) rotate([-90, -90, 0]) servo_wall_screw_mount_female();
            translate ([w2, bearing_distance + servo_distance - screw_mount_thickness, 0]) rotate([-90, -90, 0]) servo_wall_screw_mount_female();
            // switch handle (left/right style)
            color("pink") translate ([-w2/2, 0, screw_mount_width]) rotate([-90, 0, 0]) servo_wall_screw_mount_female();
        }
        // third wall servo cut
        translate ([(w2-servo_w)/2, bearing_distance + servo_distance-5.5, axis_level+servo_axis_offset-servo_h]) cube([servo_w, 2*servo_wall+1, servo_h]);
    }
}
module handle_top() {
    union() {
        difference() {
            // third wall
            translate ([0, bearing_distance + servo_distance + element_distance, 0]) cube ([w2, servo_wall, axis_level+bearing_fi/2+2*marigin]);
            translate ([(w2-servo_w)/2, bearing_distance + servo_distance-5.5 + element_distance, axis_level+servo_axis_offset-servo_h]) cube([servo_w, 2*servo_wall+1, servo_h]);
        }
        translate ([-screw_mount_width, bearing_distance + servo_distance + element_distance, 0]) rotate([-90, -90, 0]) servo_wall_screw_mount_male();
        translate ([w2, bearing_distance + servo_distance + element_distance, 0]) rotate([-90, -90, 0]) servo_wall_screw_mount_male();
    }
    translate([(w2-servo_w)/2, bearing_distance + servo_distance + element_distance + servo_wall + 0.1, axis_level+servo_axis_offset-servo_h]) union() {
        translate([-thick_side_walls_thickness, -screw_mount_thickness, -h1]) cube ([thick_side_walls_thickness, servo_mount_depth+2*screw_mount_thickness, servo_h+h1]);
        translate([-thick_side_walls_thickness + servo_w + thick_side_walls_thickness, -screw_mount_thickness, -h1]) cube ([thick_side_walls_thickness, servo_mount_depth+2*screw_mount_thickness, servo_h+h1]);
        color("blue") translate([-screw_mount_width, servo_mount_depth, 0]) rotate([-90, -90, 0]) servo_wall_screw_mount_female();
        color("red")  translate([-screw_mount_width+servo_w+screw_mount_width, servo_mount_depth, 0]) rotate([-90, -90, 0]) servo_wall_screw_mount_female();
    }
    translate ([(w2-servo_w)/2 - reinforcement_width, bearing_distance + servo_distance + element_distance + servo_wall-thick_side_walls_thickness/2, 0]) reinforcement_tower();
    translate ([(w2-servo_w)/2 + servo_w, bearing_distance + servo_distance + element_distance + servo_wall-thick_side_walls_thickness/2 + reinforcement_width, 0]) rotate([0, 0, -90]) reinforcement_tower();
    translate ([(w2-servo_w)/2, bearing_distance + servo_distance + element_distance + servo_wall-thick_side_walls_thickness/2 + servo_mount_depth-reinforcement_width*0.7, (servo_h+h1)/3+2]) rotate([0, 0, 90]) reinforcement();
    translate ([(w2-servo_w)/2, bearing_distance + servo_distance + element_distance + servo_wall-thick_side_walls_thickness/2 + servo_mount_depth-reinforcement_width*0.7, (servo_h+h1)*0.6]) rotate([0, 0, 90]) reinforcement();
    translate ([(w2-servo_w)/2, bearing_distance + servo_distance + element_distance + servo_wall-thick_side_walls_thickness/2 + servo_mount_depth-reinforcement_width*0.7, (servo_h+h1)*0.8]) rotate([0, 0, 90]) reinforcement();
    translate ([(w2-servo_w)/2 + servo_w+reinforcement_width*1.3, bearing_distance + servo_distance + element_distance + servo_wall-thick_side_walls_thickness/2 + servo_mount_depth+reinforcement_width*0.6, (servo_h+h1)/3+2]) rotate([0, 0, -180]) reinforcement();
    translate ([(w2-servo_w)/2 + servo_w+reinforcement_width*1.3, bearing_distance + servo_distance + element_distance + servo_wall-thick_side_walls_thickness/2 + servo_mount_depth+reinforcement_width*0.6, (servo_h+h1)*0.6]) rotate([0, 0, -180]) reinforcement();
    translate ([(w2-servo_w)/2 + servo_w+reinforcement_width*1.3, bearing_distance + servo_distance + element_distance + servo_wall-thick_side_walls_thickness/2 + servo_mount_depth+reinforcement_width*0.6, (servo_h+h1)*0.8]) rotate([0, 0, -180]) reinforcement();
}
module handle_lid() {
    translate([0, bearing_distance + servo_distance + 3*element_distance + servo_wall, 0]) union() {
        rotate([0, 0, 90]) servo_wall_screw_mount_male();
        translate([servo_w+screw_mount_width, 0, 0]) rotate([0, 0, 90]) servo_wall_screw_mount_male();
        cube([servo_w,w2,thick_side_walls_thickness]);
    }
}
pin_height = 4+8+12;
pin_copule_r = 4;
pin_h = 9.4;
pin_distance_ring = 6;
pin_distance_ring_r = 11.65/2;
groove_h = 18;
screw_h = 8.2;
module handle_axis() {
    difference() {
        union() {
            cylinder (pin_height, pin_h/sqrt3, pin_h/sqrt3, $fn=6);
            translate([0, 0, pin_height]) sphere(pin_copule_r, $fn=100);
            cylinder (pin_distance_ring_r, pin_distance_ring+3, pin_distance_ring+3, $fn=100);
        }
        translate([0, 0, -0.1]) cylinder (groove_h, screw_h/sqrt3, screw_h/sqrt3, $fn=6);
    }
}
switch_w = 25;
module handle_switch() {
    difference(){
        translate([0.4*w2, 0, 0])  cube ([w2, screw_mount_width, screw_mount_thickness], center=true);
        translate([0, 0, 0]) cylinder(screw_mount_width, screw_fi/2, screw_fi/2, $fn=100);
    }
    translate([w2, -switch_w/2 + screw_mount_width/2, 0]) rotate([0, 0, 90]) cube ([switch_w, screw_mount_width, screw_mount_thickness], center=true);
//    translate([w2, -switch_w/2 + screw_mount_width/2, 0]) union(){
//        color ("pink")cylinder (screw_mount_thickness+10, 2.7/2, 2.7/2, $fn=100);
//        color ("pink") translate([0, -9/2, 0]) cylinder (screw_mount_thickness+10, 2.7/2, 2.7/2, $fn=100);
//    }
}
*handle();
*handle_top();
*handle_lid();
*handle_axis();
handle_switch();