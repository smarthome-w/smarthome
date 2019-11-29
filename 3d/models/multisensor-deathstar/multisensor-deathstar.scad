// based on  https://www.thingiverse.com/thing:1284637

bottom_offset = 55;

translate ([0, 0, 4]) {
    union() {
        bottom(bottom_offset);
        holder(bottom_offset);
    }
}
top(-bottom_offset);

module holder (part_offset) {
   translate ([part_offset, 0, 0]) {
    translate ([0, 0, -4]) {
       difference() { 
            cylinder(h=6, r=46.3, $fn=100);
            translate ([0, 0, -1]) cylinder(h=12, r=44, $fn=100);
       }
   }
   }
}

module top (part_offset) {
   translate ([part_offset, 0, 0]) {
       difference() { 
           color ("blue") {
               translate ([-50, 50, 0]) {
                   rotate ([90, 0, 0]) {
                        import ("../../import/deathstar/DEATH_STAR_TOP_.02DEEP.STL");
                   }
               }    
           }
           color ("red") sphere (46.3);
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
           color ("red") sphere (46.3);
       }
   }
   }       
