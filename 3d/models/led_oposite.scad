    union() {
        union() {
            difference(){
                color("blue") cube ([20, 8, 20], center=true);
                color("red") translate ([0, -1, 0]) cube ([16, 7, 200], center=true);
            }
        }

        color("yellow")
            rotate(a=135)
                translate([7.5, 38.41, -10]) difference() {
                    cube ([30, 2, 20]);
                }

        color("pink")
            rotate(a=45)
                translate([-40.5, 7.41, -10]) difference() {
                    cube ([50, 2, 20]);
                }

        color("orange") 
            rotate(a=135) 
                translate([-4, -9.6, -10]) 
                    cube ([12, 2, 20]);
    }
