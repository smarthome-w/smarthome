difference() {
    color("red") cube ([27, 12, 20]);
    union() {
        color("yellow") translate ([-0.1,-0.1,20-7.4]) cube ([27.2, 5, 7.5]);
        color("pink") translate ([-0.1,5-0.2,20-13.4]) cube ([27.2, 3, 13.5]);
    }
}