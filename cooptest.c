#include <stdio.h>
#include <stdlib.h>

struct Rectangle {
	int width;
	int height;
	int area;
};

struct Rectangle Rectangle(int w, int h) {
struct Rectangle this;
		this.width = w;
		this.height = h;
		this.area = this.width * this.height;
	return this;
}
	Rectangle halve(int axis) {
		if (axis) {
			return Rectangle(this.width, this.height/2);
		} else {
			return Rectangle(this.width/2, this.height);
		}
	}
	char *string() {
		char *string = malloc(100);
		sprintf(string, "%i*%i", this.width, this.height);
		return string;
	}

int main() {
	Rectangle r = Rectangle(10, 20);
	Rectangle s = r.halve(1);
	printf("%s: %i", s.string(), s.area);
	return 0;
}