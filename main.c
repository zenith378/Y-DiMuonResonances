//
//  main.c
//  
//
//  Created by Giulio Cordova on 10/11/22.
//
//

#include "SpettrumPlot.h"
#include "fitlp2.h"
#include "stdio.h"

int main() {
    SpettrumPlot();
    fitlp2("hist",70,100);
}
