    union() {
        union() {
            difference(){
                color("blue") cube ([20, 8, 20], center=true);
                color("red") translate ([0, -1, 0]) cube ([16, 7, 200], center=true);
            }
        }

        color("yellow")
            rotate(a=135)
                translate([7.5, 13.41, -10]) difference() {
                    cube ([10, 2, 20]);
                }

        color("pink")
            rotate(a=45)
                translate([-15.5, 7.41, -10]) difference() {
                    cube ([25, 2, 20]);
                }

        color("orange") 
            rotate(a=135) 
                translate([-4, -9.6, -10]) 
                    cube ([12, 2, 20]);
    }
