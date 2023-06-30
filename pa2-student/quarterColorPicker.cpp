/**
 * @file quarterColorPicker.cpp
 * @description Implements the QuarterColorPicker constructor and () function
 *              for CPSC 221 2023S PA2
 *
 *              THIS FILE WILL BE SUBMITTED
 */

#include "quarterColorPicker.h"

QuarterColorPicker::QuarterColorPicker(PNG& inputimg)
{
    // Complete your implementation below
    referenceimg = inputimg;

}

/**
 * Picks the color for pixel (x, y).
 *
 * Using the private reference image, scale each dimension by half
 * and tile the smaller image in a 2 x 2 grid over the original dimensions
 * of the image, and return the pixel at the appropriate coordinate from
 * the tiled image.
 *
 * The value of each pixel in the scaled image will be a bilinear interpolation
 * of a 2x2 pixel region from the original image, with each of the H/S/L/A channels
 * processed individually. Interpolate over the x-axis before the y-axis.
 *
 * Since interpolation will be the exact average between exactly two pixels, then
 * the Hue channel should be averaged over the shortest arc between the two values.
 * e.g. The average of H_1 = 10 and H_2 = 320 is H_avg = 345 and NOT 165.
 * In the case of two diametric hues being averaged, e.g. 10 and 190, the average will
 * have the smaller value, i.e. 100 and NOT 280.
 * 
 * @pre referenceimg width and height are both even
 */
HSLAPixel QuarterColorPicker::operator()(PixelPoint p)
{
    // Replace the line below with your implementation
    int x = (p.x % (referenceimg.width() / 2)) * 2;
    int y = (p.y % (referenceimg.height() / 2)) * 2;

    // take the four pixels
    HSLAPixel* topLeft = referenceimg.getPixel(x, y);
    HSLAPixel* topRight = referenceimg.getPixel(x + 1, y);
    HSLAPixel* bottomLeft = referenceimg.getPixel(x, y + 1);
    HSLAPixel* bottomRight = referenceimg.getPixel(x + 1, y + 1);

    // interpolate the values from x axis first and y axis
    double h = CalculateHue(topLeft->h, topRight->h, bottomLeft->h, bottomRight->h);
    double s = (topLeft->s + topRight->s + bottomLeft->s + bottomRight->s) / 4.0;
    double l = (topLeft->l + topRight->l + bottomLeft->l + bottomRight->l) / 4.0;
    double a = (topLeft->a + topRight->a + bottomLeft->a + bottomRight->a) / 4.0;

    HSLAPixel pixel = HSLAPixel(h,s,l,a);
    return pixel;
}

/**
 * Add your private QuarterColorPicker function implementations below
 */
double QuarterColorPicker::CalculateHue(double h1, double h2, double h3, double h4) {
    double xHue = CalculateHue(h1, h2);
    double yHue = CalculateHue(h3, h4);
    double totalHue = CalculateHue(xHue, yHue);

    return totalHue; 
}


double QuarterColorPicker::CalculateHue(double h1, double h2){
    
    if (abs(h1 - h2) > 180.0) {
        double ans = (h1+h2+360.0)/2.0;
        if(ans > 360.0) {
            ans = ans - 360.0;
        }
        return ans;
    }
    
    if (abs(h1 - h2) == 180.0) {
        if (h1 < 90 || h2 < 90) {
            return (h1+h2)/2.0;
        } else if (h1 == 90 || h2 == 90) {
            return 0;
        } else {
            return min(h1,h2)/2.0;
        }
    }

    return (h1+h2)/2.0;
 }


