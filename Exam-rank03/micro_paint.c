#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int X;
int Y;
char **drawing;

typedef struct srect
{
    float px, py;
    float xsize, ysize;
    char type, fill;
} rect;

void print(const char *str)
{
    int a = 0;

    while (str[a])
        a++;
    write(1, str, a);
}

void freedrawing()
{
	int y = 0;

	//remember to free every string
	while (y < Y)
	{
		free(drawing[y]);
		y++;
	}

	free(drawing);
}

void paintpixel(int x, int y, rect r)
{
    if (x >= r.px && x <= r.px + r.xsize && y >= r.py && y <= r.py + r.xsize)
    {
        if (r.type == 'R')
            drawing[y][x] = r.fill;
        else
        {
            if (!(x - 1 >= r.px && x + 1 <= r.px + r.xsize && y - 1 >= r.py && y + 1 <= r.py + r.xsize))
                drawing[y][x] = r.fill;
        }
    }
}

int checkrect(FILE *file)
{

	rect r;
	int res;
	int x, y = 0;

	res = fscanf(file, "\n%c %f %f %f %f %c", &r.type, &r.px, &r.py, &r.xsize, &r.ysize, &r.fill);

	//res = -1 means file is over
	if (res == -1)
		return (0);

	//rectangle is wrong
	if (res < 6 || r.xsize <= 0 || r.ysize <= 0 || (r.type != 'r' && r.type != 'R'))
	{
		return (1);
	}

	//draw the rectangle inside drawing
	while (y < Y)
	{
		x = 0;
		while (x < X)
		{
			paintpixel(x, y, r);
			x++;
		}
		y++;
	}
	//all went well, read the next line (return something other than 1)
	return (42);
}

void drawboard()
{
	int x, y = 0;

	//prints the drawing
	while (y < Y)
	{
		x = 0;
		while (x < X)
		{
			write(1, &drawing[y][x], 1);
			x++;
		}
		write(1, "\n", 1);
		y++;
	}
}

void createbg(char bg)
{
    int x, y = 0;

    drawing = malloc(sizeof(char *) * Y);
    while (y < Y)
    {
        x = 0;
        drawing[y] = malloc(X);
        while (x < X)
        {
            drawing[y][x] = bg;
            x++;
        }
        y++;
    }
}

int checkfile(FILE *file)
{
    int ret;
    char bg;

    if (fscanf(file, "%d %d %c", &X, &Y, &bg) != 3)
        return (1);
    if (X < 1 || X > 300 || Y < 1 || Y > 300)
        return (1);
    createbg(bg);
    while ((ret = checkrect(file)))
    {
        if (ret == 1)
        {
            freedrawing();
            return (1);
        }
    }
    drawboard();
    freedrawing();
    return (0);
}

int main(int argc, char **argv)
{
    FILE *file;

    if (argc == 2)
    {
        file = fopen(argv[1], "r");
        if (file && !checkfile(file))
        {
            fclose(file);
            return (0);
        }
        if (file)
            fclose(file);
        print("Error: Operation file corrupted\n");
    }
    else
        printf("Error: argument\n");
    return (1);
}