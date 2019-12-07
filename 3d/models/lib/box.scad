//box (10, 10, 10, 0.5);

box_x = 15;
box_y = 10;
box_z = 10;
thick = 1;

for (ix = [0: 1: 10]) {
    for (iy = [0: 1: 10]) {
        translate([ix * box_x, iy * box_y, 0]) box (box_x, box_y, box_z, thick);
    }
}

module box (width, depth, height, thickness) {
    translate([width/2, depth/2, height/2]) {
        difference() {
            color ("red") cube([width, depth, height], center = true);
            translate([0, 0, thickness]) {
                color ("blue") cube([width - 2 * thickness, depth - 2 * thickness, height - thickness], center = true);
            }
        }
    }
}
