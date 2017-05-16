class Point{
	public:
		float x;
		float y;
		Point();
		Point(int, int);
		Point(float, float);
		Point operator+ (Point);
		Point operator- (Point);
};


Point :: Point(){
	x = 0;
	y = 0;
}
Point :: Point(int x, int y){
	this->x = (float)x;
	this->y = (float)y;
}
Point :: Point(float, float){
	this->x = x;
	this->y = y;
}
Point Point::operator+ (Point V){
	return Point(this->x + V.x, this->y + V.y);
}
Point Point::operator- (Point V){
	return Point(this->x - V.x, this->y - V.y);
}