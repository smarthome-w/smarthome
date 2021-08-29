// points = number of points (minimum 3)
// outer  = radius to outer points
// inner  = radius to inner points
module Star(points, outer, inner) {
	
	// polar to cartesian: radius/angle to x/y
	function x(r, a) = r * cos(a);
	function y(r, a) = r * sin(a);
	
	// angular width of each pie slice of the star
	increment = 360/points;
	
	union() {
		for (p = [0 : points-1]) {
			
			// outer is outer point p
			// inner is inner point following p
			// next is next outer point following p

			assign(	x_outer = x(outer, increment * p),
					y_outer = y(outer, increment * p),
					x_inner = x(inner, (increment * p) + (increment/2)),
					y_inner = y(inner, (increment * p) + (increment/2)),
					x_next  = x(outer, increment * (p+1)),
					y_next  = y(outer, increment * (p+1))) {
				polygon(points = [[x_outer, y_outer], [x_inner, y_inner], [x_next, y_next], [0, 0]], paths  = [[0, 1, 2, 3]]);
			}
		}
	}
}


gear_inner_fi = 16;
gear_outer_fi = 35;
gear_h = 15;
gear_groove_h = 5;
gear_star_radius = 3.1 * 32;

screw_h = 8;
sqrt3 = 1.732;
groove_h = 8;

difference() {
    difference() {
        cylinder(gear_h, gear_outer_fi/2, gear_outer_fi/2, $fn=100);

        translate([0, 0, gear_h-gear_groove_h + 0.1]) linear_extrude(height=gear_groove_h) {
            Star(6, gear_star_radius/2, gear_inner_fi/2);
        }
    }


    translate([0, 0, -0.1]) cylinder (groove_h, screw_h/sqrt3, screw_h/sqrt3, $fn=6);
}