rounded_rectangle (10, 20, 1);

module rounded_rectangle(x, y, r) {
    hull() {
        translate([r, r]) circle(r, $fn=100);
        translate([x - r, y - r]) circle(r, $fn=100);
        translate([r, y - r]) circle(r, $fn=100);
        translate([x -r, r]) circle(r, $fn=100);
    }
}