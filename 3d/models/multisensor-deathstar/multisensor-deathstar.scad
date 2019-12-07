// based on  https://www.thingiverse.com/thing:1284637

bottom_offset = 55;

translate ([0, 0, 1]) {
    union() {
        bottom(bottom_offset);
        holder(bottom_offset);
    }
}
top(-bottom_offset);

module holder (part_offset) {
   translate ([part_offset + 1.5, 0, 2.8]) {
    translate ([0, 0, -4]) {
       difference() { 
            cylinder(h=2, r=48.3, $fn=100);
            translate ([0, 0, -1]) cylinder(h=12, r=48, $fn=100);
       }
   }
   }
}

module top (part_offset) {
   translate ([part_offset, 0, 0]) {
       difference() { 
           color ("blue") {
               translate ([-50, 50, -1]) {
                   rotate ([90, 0, 0]) {
                        import ("../../import/deathstar/DEATH_STAR_TOP_.02DEEP.STL");
                   }
               }    
           }
           color ("red") translate ([1.25, -1.5, 0]) sphere (49.5);
       }
   }
   }       

module bottom (part_offset) {
   translate ([part_offset, 0, 0]) {
       difference() { 
           color ("blue") {
               translate ([-50, -50, 50]) {
                   rotate ([-90, 0, 0]) {
                        import ("../../import/deathstar/DEATH_STAR_BOTTOM_.02DEEP.STL");
                   }
               }    
           }
           color ("red") translate ([1.25, 1.0, 0]) sphere (49.5);
       }
   }
   }       
