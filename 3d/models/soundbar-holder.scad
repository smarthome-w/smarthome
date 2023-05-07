    union() {
        union() {
            difference(){
                color("blue") cube ([25, 85, 25], center=true);
                color("red") translate ([0, -3, 0]) cube ([19, 85, 200], center=true);
            }
        }

        color("yellow")
            difference(){
                translate([17, 0, 0]) difference() {
                    cube ([10, 85, 25], center=true);
                }
                rotate(a=110) translate([-65, -32, 0]) union() {
                        color("blue") cube ([80, 40, 25.1], center=true);
                    }
                }


        rotate(a=110)
            translate([-64, -31, 0]) union() {
                difference(){
                    color("blue") cube ([80, 40, 24.9], center=true);
                    color("red") translate ([-6.1, -6, 0]) cube ([80, 40, 200], center=true);
                }
            }





        *color("yellow")
            rotate(a=135)
                translate([7.5, 38.41, -10]) difference() {
                    cube ([30, 2, 20]);
                }

        *color("pink")
            rotate(a=45)
                translate([-40.5, 7.41, -10]) difference() {
                    cube ([50, 2, 20]);
                }

        *color("orange") 
            rotate(a=135) 
                translate([-4, -9.6, -10]) 
                    cube ([12, 2, 20]);
    }
