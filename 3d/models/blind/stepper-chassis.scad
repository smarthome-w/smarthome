use <../lib/2d.scad>
use <../lib/3d.scad>
use <../lib/box.scad>

w1 = 40.55;
w2 = 44.9;
h1 = 5.4;
h2 = 1.75;
r = 1;
blind_groove_len = 50;
toolbox_mount = 25;
h = 14;
l = 33;
w = 43.5;
t = 2;
p = 3;
facet_thickness = 2;
k = 24;
mount_screw_distance = 31;
screw3_phi = 1.5;

space_diameter = 31;

cut_marigin = 10;

module handle() {
    difference() {
        union() {
            translate([0, -blind_groove_len, 0]) union() {
                translate([(w2 - w1) / 2, 0, 0]) color("blue") cube([w1, blind_groove_len + toolbox_mount + w1, h1]);
                translate([0, 0, h1]) color("red") rounded_cube(w2, blind_groove_len + r + toolbox_mount + w1, h2, r);
                s = 5;
                translate([0, r + s, h1 + h2]) cube([2, blind_groove_len - r - s, 4]);
                translate([w2 - 2, r + s, h1 + h2]) cube([2, blind_groove_len - r - s, 4]);
            }
            translate([(w2 - w) / 2, facet_thickness, 0]) color("red") union() {
                union() {
                    translate([-t, 0, 0]) color("blue") cube([t, l, h + w + p]);
                    cube([w, l, h]);
                    translate([w, 0, 0]) color("blue") cube([t, l, h + w + p]);
                }
                difference() {
                    translate([-t, -facet_thickness, 0]) cube([t + t + w, facet_thickness, w + h + p]);
                    * translate([(w - k) / 2, -facet_thickness - 1, h + k / 2]) cube([k, facet_thickness + 2, w + 100]);
                    translate([(w - k) / 2 - screw3_phi - screw3_phi, 1, h + (w - k) / 2]) rotate([90, 0, 0]) union() {
                        cylinder(h = facet_thickness + cut_marigin, r = screw3_phi, $fn = 100, center = true);
                        translate([mount_screw_distance, 0, 0]) cylinder(h = facet_thickness + cut_marigin, r = screw3_phi, $fn = 100, center = true);
                        translate([mount_screw_distance, mount_screw_distance, 0]) cylinder(h = facet_thickness + cut_marigin, r = screw3_phi, $fn = 100, center = true);
                        translate([mount_screw_distance, mount_screw_distance, 0]) cylinder(h = facet_thickness + cut_marigin, r = screw3_phi, $fn = 100, center = true);
                    }
                }
            }
        }
        translate([w2 / 2, 0, h + (w - k) / 2 + space_diameter / 2]) rotate([90, 0, 0]) cylinder(h = 100, r = space_diameter / 2, $fn = 100, center = true);
    }
}

solenoid_width = 10.5;
solenoid_len = 14;
solenoid_height = 8.65;

hole_marigin = 1;
screw_ring = screw3_phi + hole_marigin;

hat_pin_phi = 4.3;
hat_pin_wall_thick = 1;

hat_ring = hat_pin_phi + hat_pin_wall_thick;

solenoid_base_width = solenoid_width + 2 * screw_ring;
solenoid_base_len = solenoid_width + screw_ring;
solenoid_base_shift = 10;
melting_marigin = 0.3;
marigin = 0.3;
protection_len = 6;
holder_axis = mount_screw_distance/2;

module solenoid_holder() {
    difference() {
        union() {
            color ("blue") translate([holder_axis-solenoid_width/2-screw_ring/2-1, solenoid_base_len/2+solenoid_base_shift, facet_thickness-melting_marigin]) cylinder(h=solenoid_height, r=screw_ring, $fn=100);
            color ("blue") translate([holder_axis+solenoid_width/2+screw_ring/2+1, solenoid_base_len/2+solenoid_base_shift, facet_thickness-melting_marigin]) cylinder(h=solenoid_height, r=screw_ring, $fn=100);

            color("green") translate([holder_axis-facet_thickness/2-hat_ring/2-marigin-1, 0, facet_thickness-melting_marigin]) cube([facet_thickness/2, protection_len, solenoid_height]);
            color("green") translate([holder_axis+hat_ring/2+marigin+1, 0, facet_thickness-melting_marigin]) cube([facet_thickness/2, protection_len, solenoid_height]);

            color("red") color("green") translate([holder_axis-hat_ring/2-marigin-1, protection_len/2, 0]) rotate([0,0,90]) cube([facet_thickness/2, protection_len, solenoid_height]);
            color("red") color("green") translate([holder_axis+hat_ring/2+protection_len+marigin+1, protection_len/2, 0]) rotate([0,0,90]) cube([facet_thickness/2, protection_len, solenoid_height]);

            *   color("red") translate([holder_axis, 0, facet_thickness-melting_marigin]) cube([hat_ring, protection_len, solenoid_height], center=true);

            translate([-facet_thickness+holder_axis-solenoid_width/2, solenoid_base_shift, facet_thickness-melting_marigin]) cube([facet_thickness, solenoid_len-screw_ring/2, solenoid_height]);
            translate([holder_axis+solenoid_width/2, solenoid_base_shift, facet_thickness-melting_marigin]) cube([facet_thickness, solenoid_len-screw_ring/2, solenoid_height]);

            translate([0,0,facet_thickness/2]) hull() {
                cylinder(h = facet_thickness, r = screw_ring, $fn = 100, center = true);
                translate([mount_screw_distance, 0, 0]) cylinder(h = facet_thickness, r = screw_ring, $fn = 100, center = true);
                translate([holder_axis, solenoid_base_len/2+solenoid_base_shift, 0]) cube([solenoid_base_width, solenoid_base_len, facet_thickness], center = true);
            }
        }

        color ("red") union() {
            translate([holder_axis-solenoid_width/2-screw_ring/2-1, solenoid_base_len/2+solenoid_base_shift, facet_thickness+melting_marigin]) cylinder(h=solenoid_height, r=screw3_phi, $fn=100);
            translate([holder_axis+solenoid_width/2+screw_ring/2+1, solenoid_base_len/2+solenoid_base_shift, facet_thickness+melting_marigin]) cylinder(h=solenoid_height, r=screw3_phi, $fn=100);
        }

        union() {
            cylinder(h = facet_thickness + cut_marigin, r = screw3_phi, $fn = 100, center = true);
            translate([mount_screw_distance, 0, 0]) cylinder(h = facet_thickness + cut_marigin, r = screw3_phi, $fn = 100, center = true);
        }
    }
}

module solenoid_clamp() {
    difference() {
        hull() {
            translate([holder_axis-solenoid_width/2-screw_ring/2-1, solenoid_base_len/2, 0]) cylinder(h=facet_thickness, r=screw_ring, $fn=100);
            translate([holder_axis+solenoid_width/2+screw_ring/2+1, solenoid_base_len/2, 0]) cylinder(h=facet_thickness, r=screw_ring, $fn=100);
        }

        color ("red") union() {
            translate([holder_axis-solenoid_width/2-screw_ring/2-1, solenoid_base_len/2, -melting_marigin]) cylinder(h=facet_thickness+cut_marigin, r=screw3_phi, $fn=100);
            translate([holder_axis+solenoid_width/2+screw_ring/2+1, solenoid_base_len/2, -melting_marigin]) cylinder(h=facet_thickness+cut_marigin, r=screw3_phi, $fn=100);
        }
    }
}

handle();
*solenoid_holder();
*solenoid_clamp();