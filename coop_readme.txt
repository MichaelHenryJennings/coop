Specifications:

All class members should be isolated into a struct
Constructors should be replaced with methods returning the struct 
Methods should be replaced with methods taking the struct as a parameter 
All references to class members inside the class should be prepended by "this." unless already the case
Object declarations should be replaced by struct declarations
Instance method calls (including constructions) should be replaced by calls to the appropriate function.
Dot-operator prefixes of function calls should be shunted inside as the first argument.

Hence,
---------------------------------------------------------------
class Rectangle {
	int width;
	int height;
	Rectangle(int width, int height) {
		this.width = width;
		this.height = height;
		area = width * height;
	}
	int area;
	Rectangle halve(int axis) {
		if (axis > 0) {
			return Rectangle(width, height/2);
		} else {
			return Rectangle(width/2, height);
		}
	}
	char *string() {
		char *string = malloc(100);
		sprintf(string, "%s*%s", width, height);
		return string;
	}
}
int main() {
	Rectangle r = Rectangle(10, 20);
	Rectangle s = r.halve(1);
	printf("%s: %i", s.string(), s.area);
}
---------------------------------------------------------------
should become:
---------------------------------------------------------------
struct Rectangle {
	int width;
	int height;
	int area;
}
struct Rectangle Rectangle(int width, int height) {
	struct Rectangle this;
	this.width = width;
	this.height = height;
	this.area = width * height;
	return this;
}
struct Rectangle halve(struct Rectangle this, boolean axis) {
	if (axis > 0) {
		return Rectangle(this.width, this.height/2);
	} else {
		return Rectangle(this.width/2, this.height);
	}
}
char *string(struct Rectangle this) {
	char *string = malloc(100);
	sprintf(string, "%s*%s", this.width, this.height);
	return string; 
}
int main() {
	struct Rectangle r = Rectangle(10, 20);
	struct Rectangle s = halve(r, 1);
	printf("%s: %i", string(s), s.area);
}

---------------------------------------------------------------
Notes:
Keywords (should not be used elsewhere): class, this
main() should not be declared inside a class.
Class names cannot conflict with existing struct or function definitions
Likewise, object names cannot conflict with existing struct instances or variable names
Nested classes are currently not supported
Classes must be declared before use

Possible future features:
Nested & function-internal classes
Usage before declaration
Cross-file class usage
Bootstrapped compiler
Error handling

Efficiency notes:
Is reading the entire file ahead of time necessary?
Use buffered/optimized reader?
Faster way to pattern match?