#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int red;
    int green;
    int blue;
} COLOR;

typedef struct
{
    int width;
    int height;
    COLOR *raster;
} IMAGE;

// Creates and returns a pointer to a new blank IMAGE
// with the given dimensions (allocated on the heap)
IMAGE *img_new(int width, int height)
{
    IMAGE *img = malloc(sizeof(IMAGE));
    img->width = width;
    img->height = height;
    img->raster = malloc(sizeof(COLOR) * width * height);
    return img;
}

// Frees the memory of the given IMAGE
void img_free(IMAGE *img)
{
    //free(img->raster);
    //free(img);
    if (img)
    {
        if (img->raster)
        {
            free(img->raster);
        }
        free(img);
    }
}

IMAGE *img_read(const char *filename)
{
    FILE *fpointer = fopen(filename, "r");
    char buffer[100];

    // 1st line
    fgets(buffer, 100, fpointer);
    // 2nd line for width and height
    fgets(buffer, 100, fpointer);
    int w;
    int h;
    // get width and height from the file and store it
    sscanf(buffer, "%d %d", &w, &h);
    //printf("dimension is %d x %d", w, h);
    IMAGE *imagepointer = img_new(w, h);
    // skip third line 255
    fgets(buffer, 100, fpointer);
    // populate raster
    int i;
    for (i = 0; i < w * h; i++)
    {
        fgets(buffer, 100, fpointer);
        sscanf(buffer, "%d %d %d", &imagepointer->raster[i].red,
               &imagepointer->raster[i].green, &imagepointer->raster[i].blue);
    }

    // for (i = 0; i < 100; i++)
    // {
    //     printf("%d %d %d\n", imagepointer->raster[i].red,
    //            imagepointer->raster[i].green, imagepointer->raster[i].blue);
    // }

    fclose(fpointer);
    return imagepointer;
    //img_new(w, h);
}

void img_write(IMAGE *img, const char *filename)
{
    FILE *fpointer = fopen(filename, "w");
    // First line
    fprintf(fpointer, "P3\n");
    // Get width and height value from POINT struct pointer and write it
    fprintf(fpointer, "%d %d\n", img->width, img->height);
    // Third line 255
    fprintf(fpointer, "255\n");
    // the rest of the line, pixels
    int i;
    int w = img->width;
    int h = img->height;
    for (i = 0; i < w * h; i++)
    {
        fprintf(fpointer, "%d %d %d\n", img->raster[i].red,
                img->raster[i].green, img->raster[i].blue);
    }
    fclose(fpointer);
}

int gray_color(IMAGE *img, int pixel)
{
    // get the three color first
    int red = img->raster[pixel].red;
    int blue = img->raster[pixel].blue;
    int green = img->raster[pixel].green;
    int gray = (red + blue + green) / 3;
    return gray;
}

void img_gray(IMAGE *img)
{
    int i;
    int w = img->width;
    int h = img->height;
    // loop over raster to get color
    for (i = 0; i < w * h; i++)
    {
        // convert to color gray first
        int gray = gray_color(img, i);
        // replace rgb with gray color pixel in the raster
        img->raster[i].red = gray;
        img->raster[i].green = gray;
        img->raster[i].blue = gray;
    }
}

int main(int argc, char const *argv[])
{
    if (argc == 3)
    {
        IMAGE *img = img_read(argv[1]);
        img_gray(img);
        img_write(img, argv[2]);
        img_free(img);
        return 0;
    }
    else
    {
        printf("Error! Needs exactly two arguments.\n");
        return 1;
    }
}