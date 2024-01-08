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
	struct Rectangle halve(struct Rectangle this, int axis) {
		if (axis) {
			return Rectangle(this.width, this.height/2);
		} else {
			return Rectangle(this.width/2, this.height);
		}
	}
	char *string(struct Rectangle this) {
		char *string = malloc(100);
		sprintf(string, "%i*%i", this.width, this.height);
		return string;
	}

int main() {
	struct Rectangle r = Rectangle(10, 20);
	struct Rectangle s = halve(r, 1);
	printf("%s: %i", string(s), s.area);
	return 0;
}