sqrt3 = 1.732;
pin_h = 9.45;
pin_l = 45;


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


module wheel_teeth(teeths, teeth_thick, inner, outer, height) {
    union() {
        for (p = [0 : teeths-1]) {
            rotate([0,0,p*360/teeths]) translate([-teeth_thick/2, 0, 0]) cube([teeth_thick, outer, height]);
        }
        cylinder (height, inner, inner, $fn=100);
    }
}

star_nodes = 20;
star_outer = 25/2;
star_inner = 21/2;
star_height = 10;

module pin() {
    difference() {
        union() {
            translate ([0, 0, 0]) union() {
                cylinder (pin_l, pin_h/sqrt3, pin_h/sqrt3, $fn=6);
                translate([0, 0, pin_l]) difference() {
                    sphere (9.45/2, $fn=100);
                    translate([0, 0, -5]) cube ([10,10,10], center=true);
                }
            }
//            translate ([0, 0, 0]) linear_extrude(height=star_height) Star(star_nodes, star_outer, star_inner);
        wheel_teeth (star_nodes, 1, star_inner, star_outer, star_height);
        }
        translate([0, 0, -0.2]) color ("red") difference() {
            cylinder (18, 2.7, 2.7, $fn=100);
            translate([-5, 2.3, -1]) cube ([10, 10, 20]);
        }
    }
}

hat_pin_l = 5.5+0.3;
hat_pin_phi = 4.1;
hat_pin_wall_thick = 1.7;
alpha_2 = 360/star_nodes/2;
hole_r = 0.8;

pick_h = star_outer - star_inner * cos(alpha_2);
pick_r = star_inner * sin(alpha_2);
module hat() {
    difference() {
        cylinder (hat_pin_l+hat_pin_wall_thick, hat_pin_phi/2+hat_pin_wall_thick, hat_pin_phi/2+hat_pin_wall_thick, $fn=100);
        translate([0,0,-0.1]) cylinder (hat_pin_l, hat_pin_phi/2, hat_pin_phi/2, $fn=100);

        translate([0,0,-0.1]) cylinder (2*hat_pin_l, hole_r, hole_r, $fn=100);
    }

}

*pin();
rotate ([0, 180, 0]) hat();

