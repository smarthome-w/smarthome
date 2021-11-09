    union() {
        union() {
            difference(){
                color("blue") cube ([20, 8, 20], center=true);
                color("red") translate ([0, -1, 0]) cube ([16, 7, 200], center=true);
            }
        }

        color("pink")
            rotate(a=45)
                translate([-4, 7.41, -10]) difference() {
                    cube ([50, 1, 20]);
                    hull(){
                        translate([60, 0, 10]) rotate([90, 0, 0]) cylinder (h = 10, r = 2, $fn = 100, center=true);
                        translate([40, 0, 10]) rotate([90, 0, 0]) cylinder (h = 10, r = 2, $fn = 100, center=true);
                    }
                }

        color("orange") 
            rotate(a=135) 
                translate([-4, -9.6, -10]) 
                    cube ([12, 2, 20]);
    }
