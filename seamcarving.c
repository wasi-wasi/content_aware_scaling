#include "seamcarving.h"
#include <math.h>
#include <stdio.h>


// Part 1: Dual-Gradient Energy Function

// Helper Function: To square numbers 
int square(int num)
{
    return (num * num);
}

// Boolean Helper Function to determine the remaining index values to be calculate

void calc_energy(struct rgb_img *im, struct rgb_img **grad)
{
    // Coordinate Plane: y - vertical, x - horizontal: (y, x)
    size_t height = im->height;
    size_t width = im->width;

    // Create the energy gradient image
    create_img(grad, height, width);

    // Incrementing along the height of the array
    for (int y = 0; y <= height - 1; y++)
    {
        // Incrementing along the width of the array
        for (int x = 0; x <= width - 1; x++)
        {
            // Points surrounding the current point
            int above_y, below_y, right_y, left_y;
            int above_x, below_x, right_x, left_x;
            
            if (y == 0 && x == 0) // {Top left corner}
            {
                // Calculate using points
                // Above: (height - 1 [last row], 0 [first column])
                above_y = height - 1;
                above_x = 0; 
                // Left: (0 [ first row], width - 1 [last column])
                left_y = 0;
                left_x = width - 1;
                // Normal Points:
                below_y = y + 1;
                below_x = x;
                right_y = y;
                right_x = x + 1;
            } else if ((y == (height - 1)) && (x == (width - 1))) // {Bottom right corner}
            {
                // Calculate using points )
                // Right: (y [height - 1], 0 [first column])
                right_y = height - 1;
                right_x = 0;
                // Below: (0 [first row], x [width - 1] )
                below_y = 0;
                below_x = width - 1;
                // Normal Points
                left_y = y;
                left_x = x - 1;
                above_y = y - 1;
                above_x = x;
            } else if (y == 0 && x == width - 1) // {Top right corner}
            {
                // Calculate using points:
                // Right: (y [first row], 0 [first column])
                right_y = y;
                right_x = 0;
                // Above: (height - 1[last row], x [last column])
                above_y = height - 1;
                above_x = x;
                // Normal Points
                left_y = y;
                left_x = x - 1;
                below_y = y + 1;
                below_x = x;
            }else if (y == height - 1 && x == 0) // {Bottom Left Corner}
            {
                // Calculate using points:
                // Left: ( y [last row], width - 1 [last column])
                left_y = y;
                left_x = width - 1;
                // Below: ( 0 [first row] , 0 [first column])
                below_y = 0;
                below_x = 0;
                //Normal Points
                right_y = y;
                right_x = x + 1;
                above_y = y - 1;
                above_x = x;
            } else if (y != 0 && x == 0) // {First Column}
            {
                // Calculate using points:
                // Left: (y [current row], width - 1[last coloumn])
                left_y = y;
                left_x = width - 1;
                // Normal Points;
                right_y = y;
                right_x = x + 1;
                above_y = y - 1;
                above_x = x;
                below_y = y + 1; 
                below_x = x;
            } else if (y != 0 && x == width - 1) // {Last Column}
            {
                // Calculate using points: 
                // Right: (y [current row], 0 [first column])
                right_y = y;
                right_x = 0;
                // Normal Points
                left_y = y;
                left_x = x - 1;
                above_y = y - 1;
                above_x = x;
                below_y = y + 1;
                below_x = x;
            } else if (y == 0 && x != 0) // {Top Row}
            {
                // Calculate using points: 
                // Above: (height - 1 [last row], x [current column])
                above_y = height - 1;
                above_x = x;
                // Normal Points
                below_y = y + 1;
                below_x = x;
                right_y = y;
                right_x = x + 1;
                left_y = y;
                left_x = x - 1;
            } else if (y == height - 1 && x != 0) // {Bottom Row}
            {
                // Calculate using the points:
                // Below: (0 [first row], x [current column])
                below_y = 0;
                below_x = x;
                // Normal Points
                above_y = y - 1;
                above_x = x;
                right_y = y;
                right_x = x + 1;
                left_y = y;
                left_x = x - 1;
            } else if (x != 0 && y != 0 && x != width - 1 && y != height - 1)
            {
                above_y = y - 1;
                above_x = x;
                below_y = y + 1;
                below_x = x;
                right_y = y;
                right_x = x + 1;
                left_y = y;
                left_x = x - 1;
            }

            //printf("y: %d, x: %d\n", y, x);
            //printf("above : (%d, %d) below: (%d, %d), right: (%d, %d), left: (%d, %d)\n", above_y, above_x, 
            //       below_y, below_x, right_y, right_x, left_y, left_x);

            // Calculate the vertical differences
            int pix_r_y, pix_g_y, pix_b_y;

            pix_r_y = get_pixel(im, below_y, below_x, 0) - get_pixel(im, above_y, above_x, 0);
            pix_g_y = get_pixel(im, below_y, below_x, 1) - get_pixel(im, above_y, above_x, 1);
            pix_b_y = get_pixel(im, below_y, below_x, 2) - get_pixel(im, above_y, above_x, 2);

            // Calculate horizontal distances between pixels
            int pix_r_x, pix_g_x, pix_b_x;

            pix_r_x = get_pixel(im, right_y, right_x, 0) - get_pixel(im, left_y, left_x, 0);
            pix_g_x = get_pixel(im, right_y, right_x, 1) - get_pixel(im, left_y, left_x, 1);
            pix_b_x = get_pixel(im, right_y, right_x, 2) - get_pixel(im, left_y, left_x, 2);

            
            // Calculate delta_y and delta_x
            int delta_y, delta_x;

            delta_y = square(pix_r_y) + square(pix_g_y) + square(pix_b_y);
            delta_x = square(pix_r_x) + square(pix_g_x) + square(pix_b_x);

            // Calculate the original energy as (delta_x + delta_y)^1/2
            int org_energy;
            org_energy = sqrt(delta_x + delta_y);

            // Adjust energy value: divide by 10
            int adj_energy;
            adj_energy = org_energy/10;

            // Cast to unit8_t
            uint8_t fin_energy;
            fin_energy = (uint8_t) adj_energy;
            
            // Store at the point in the x y coordinate in the raster
            set_pixel(*grad, y, x, fin_energy, fin_energy, fin_energy);
        }
    }
} 

// Part 2: Cost Array

double min_2(double x, double y){
  return (x < y) ? x : y;
}

double min_3(double x, double y, double z) {

  double min = 99999.000;

  if (x < min)
    min = x;
  if (y < min)
    min = y;
  if(z < min)
    min = z;

  return min;
}

void dynamic_seam(struct rgb_img *grad, double **best_arr)
{
    // Extracting the height and width of grad
    size_t height = grad->height; // y axis - rows
    size_t width = grad->width; // x axis - columns

    // Defining Variables
    double min_energy, x1, x2, x3;

    // Allocating memory for best_array
    *best_arr = (double *)malloc(height * width * sizeof(double));
    
    // Extracting energies from grad
    for (int iheight = 0; iheight < height; iheight++)
    {
       for (int iwidth = 0; iwidth < width; iwidth++)
       {
           (*best_arr)[(iheight * width) + iwidth] = get_pixel(grad, iheight, iwidth, 0);
           //printf("%lf\n", (*best_arr)[(iheight * width) + iwidth]);
       }
    }

    // Calculating the lowest energy paths
    for (int y = 1; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (x == 0)
            {
                x1 = (*best_arr)[((y - 1) * width) + x];
                x2 = (*best_arr)[((y - 1) * width) + (x + 1)];
                min_energy = min_2(x1, x2);
                //printf("%d\n", min_energy);

            } else if ( x == (width - 1))
            {
                x1 = (*best_arr)[((y - 1) * width) + (x - 1)];
                x2 = (*best_arr)[((y - 1) * width) + x];
                min_energy = min_2(x1, x2);
                //printf("%d\n", min_energy);

            } else {
                x1 = (*best_arr)[((y - 1) * width) + (x - 1)];
                x2 = (*best_arr)[((y - 1) * width) + x];
                x3 = (*best_arr)[((y - 1) * width) + (x + 1)];
                min_energy = min_3(x1, x2, x3);
                //printf("%d\n", min_energy);
            } 
    
            // Summing the min_energy of a path
            (*best_arr)[(y * width) + x] += min_energy;
            //printf("%lf\t%lf\n", (*best_arr)[(y * width) + x], min_energy);
           
        }
    }
}

// Part 3: Recover the seam
void recover_path(double *best, int height, int width, int **path)
{
    int value = 999999999;
    int index, count = 0;

    // Allocating memory for the path
    *path = (int *)malloc(height * sizeof(int));

    count = height - 1;

    // Finding the smallest value in the first row
    for (int x = 0; x < width; x++)
    {
        if (best[(height - 1) * width + x] < value)
        {
            index = x;
            value = best[(height - 1) * width + x];
        }
    }
    (*path)[count] = index;

    // Finding the minimum energy in each row
    int x1, x2, x3, min_energy;
    for (int y = height - 1; y > 0; y--)
    {
        count = count - 1;
        int x = index;
        if (x == 0)
        {
            x1 = best[((y - 1) * width) + x];
            x2 = best[((y - 1) * width) + (x + 1)];
            min_energy = min_2(x1, x2);
            if (min_energy == x1)
            {
                index = x;
            }else{
                index = x + 1;
            }
            
        } else if ( x == (width - 1))
        {
            x1 = best[((y - 1) * width) + (x - 1)];
            x2 = best[((y - 1) * width) + x];
            min_energy = min_2(x1, x2);
            if (min_energy == x1)
            {
                index = x - 1;
            }else{
                index = x;
            }

        } else {
            x1 = best[((y - 1) * width) + (x - 1)];
            x2 = best[((y - 1) * width) + x];
            x3 = best[((y - 1) * width) + (x + 1)];
            min_energy = min_3(x1, x2, x3);
            if (min_energy == x1)
            {
                index = x - 1;
            }else if (min_energy == x2)
            {
                index = x;
            } else {
                index = x + 1;
            }
        }
        (*path)[count] = index;
    }
}

// Part 4: Write a function that removes the seam
void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path)
{
    size_t height, width, n_height, n_width;
    
    // Determining the height of destination source
    height = src->height;
    width = src->width;

    n_height = height;
    n_width = width - 1;

    // Creating the destination image
    create_img(dest, n_height, n_width);
  

    // Copying over pixels from src to dest
    int rem_ind, pix_src_r, pix_src_g, pix_src_b, x_dest = -1;
    for (int y = 0; y < height; y++)
    {
        rem_ind = path[y];
        for (int x = 0; x < width; x++)
        {
            // Get pixel from source file
            pix_src_r = get_pixel(src, y, x, 0);
            pix_src_g = get_pixel(src, y, x, 1);
            pix_src_b = get_pixel(src, y, x, 2);
            
            // Set pixel in detination file
            if ( x != rem_ind)
            {
                if (x_dest < n_width - 1)
                {
                    x_dest += 1;
                } else {
                    x_dest = 0;
                }
                set_pixel(*dest, y, x_dest, pix_src_r, pix_src_g, pix_src_b);
            }
        }
    }
}
