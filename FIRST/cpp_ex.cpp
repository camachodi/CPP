#include <iostream>
#include <conio.h>

using namespace std;

void first()
{

	cout << "HOLA MUNDO :D\n";

	printf("HELLO WORLD");

}

void second()
{

	int number = 10;
	float decimal = 15.9f;
	double big = 23.3244;
	char character = 'x';

	printf("%d\n", number);
	printf("%f\n", decimal);
	printf("%f\n", big);
	printf("%c\n", character);

	cout<<number<<endl;
	cout<<decimal<<endl;
	cout<<big<<endl;
	cout<<character<<endl;

}

void third()
{

	int number;
	float numberf;

	cout<<"Enter a number: "<<endl;
	cin>>numberf;
	cout<<"Your number is:"<<numberf<<endl;
	cout<<"That is all"<<endl;

}

void fourth_one()
{
	//This is exercise 1

	float n1, n2;

	cout<<"Enter first number: "<<endl;
	cin>>n1;
	cout<<"Enter second number: "<<endl;
	cin>>n2;

	cout<<"Add result: "<<n1+n2<<endl;
	cout<<"Subs result: "<<n1-n2<<endl;
	cout<<"Mult result: "<<n1*n2<<endl;
	cout<<"Div result: "<<n1/n2<<endl;

}

void fourth_two()
{

	//This is exercise 2
	//Read price of product given by user and then show the price with the IVA

	float n1;

	cout<<"Enter the price: "<<endl;
	cin>>n1;
	cout<<"The price with IVA is:"<<n1+(n1*0.21f)<<endl;
	cout<<"That is all"<<endl;

}

void five()
{

	//This is exercise 1
	//Read age, gender and height in meters given by user and show them in console

	int age;
	char gender[10];
	float height;

	cout<<"Enter your age: "<<endl;
	cin>>age;
	cout<<"Enter your gender:"<<endl;
	cin>>gender;
	cout<<"Enter your height in meters:"<<endl;
	cin>>height;
	cout<<"Your age is: "<<age<<endl;
	cout<<"Your gender is: "<<gender<<endl;
	cout<<"Your height is: "<<height<<endl;
	cout<<"That is all"<<endl;

}

void six_one()
{
	//Write (a/b)+1 as expression in console
	
	float a, b;

	cout<<"Enter a value"<<endl;
	cin>>a;
	cout<<"Enter b value"<<endl;
	cin>>b;

	cout.precision(2);
	cout<<"The result of ("<<a<<"/"<<b<<") + 1 is: "<<(a/b) + 1<<endl;
	cout<<"That is all"<<endl;

}

void six_two()
{
	//Write (a+b)/(c+d) as expression in console

	float a,b,c,d;

	cout<<"Enter a value:"<<endl;
	cin>>a;
	cout<<"Enter b value:"<<endl;
	cin>>b;
	cout<<"Enter c value:"<<endl;
	cin>>c;
	cout<<"Enter d value:"<<endl;
	cin>>d;
	cout<<"The result of ("<<a<<"+"<<b<<")/("<<c<<"+"<<d<<") is: "<<(a+b)/(c+d)<<endl;

}

void seven_one()
{

	//Write (a + (b/c))/(d + (e/f)) as expression in console

	float a, b, c, d, e, f, res;

	cout<<"Enter a value:"<<endl;
	cin>>a;
	cout<<"Enter b value:"<<endl;
	cin>>b;
	cout<<"Enter c value:"<<endl;
	cin>>c;
	cout<<"Enter d value:"<<endl;
	cin>>d;
	cout<<"Enter e value:"<<endl;
	cin>>e;
	cout<<"Enter f value;"<<endl;
	cin>>f;

	res = (a+(b/c))/(d+(e/f));

	cout<<"The result of (a + (b/c))/(d + (e/f)) is: "<<res<<endl;
	cout<<"That is all";

}

void seven_two()
{

	//Write a + (b/(c-d)) as expression in console

	float a, b, c, d, res;

	cout<<"Enter a value:"<<endl;
	cin>>a;
	cout<<"Enter b value:"<<endl;
	cin>>b;
	cout<<"Enter c value:"<<endl;
	cin>>c;
	cout<<"Enter d value:"<<endl;
	cin>>d;

	res = a + (b / (c - d));

	cout<<"The result of a + (b/(c-d)) is: "<<res<<endl;
	cout<<"That is all"<<endl;

}

void eight_one()
{
	//Store 2 values in 2 variables, and then swap it

	float a, b, auxV;

	cout<<"Enter a value"<<endl;
	cin>>a;
	cout<<"Enter b value"<<endl;
	cin>>b;

	cout<<"a = "<<a<<endl;
	cout<<"b = "<<b<<endl;

	auxV = b;
	b = a;
	a = auxV;

	cout<<"\na = "<<a<<endl;
	cout<<"b = "<<b<<endl;
	cout<<"That is all"<<endl;
}

void eight_two()
{

	float a, b, c, d, auxV;

	cout<<"Enter a value"<<endl;
	cin>>a;
	cout<<"Enter b value"<<endl;
	cin>>b;
	cout<<"Enter c value"<<endl;
	cin>>c;
	cout<<"Enter d value"<<endl;
	cin>>d;

	auxV = (a+b+c+d)/4;

	cout<<"The average between a b c and d is: "<<auxV<<endl;
	cout<<"That is all"<<endl;

}

void nine_one()
{

	float a, b, c, auxV;

	cout<<"Enter a value that is 10% of total"<<endl;
	cin>>a;
	cout<<"Enter b value that is 60% of total"<<endl;
	cin>>b;
	cout<<"Enter c value that is 10% of total"<<endl;
	cin>>c;

	auxV = a * 0.3f + b * 0.6f + c * 0.1f;

	cout<<"The average between a b and c is: "<<auxV<<endl;
	cout<<"That is all"<<endl;

}

void nine_two()
{

	float a, b, c;

	cout<<"Enter the value of a side from a triangle"<<endl;
	cin>>a;
	cout<<"Enter the value of b side from a triangle"<<endl;
	cin>>b;

	c = sqrt(a * a + b * b);

	cout<<"The size of hypotenuse with a and b sides is: "<<c<<endl;
	cout<<"That is all"<<endl;

}

void ten_one()
{

	float a, b, auxV;

	cout<<"Enter a value"<<endl;
	cin>>a;
	cout<<"Enter b value"<<endl;
	cin>>b;

	auxV = (sqrt(a))/((b * b) - 1);

	cout<<"The result of square root of a divided by b plus b minus 1 is: "<<auxV<<endl;
	cout<<"That is all"<<endl;

}

void ten_two()
{

	//float a, b, c, x, auxV;
	//
	//cout << "Enter a value" << endl; 
	//cin>>a;
	//cout << "Enter b value" << endl; 
	//cin>>b;
	//cout << "Enter c value" << endl; 
	//cin>>c;
	//
	//x = ((-b) + (sqrt((b * b) - 4 * a * c))) / (2 * a);
	//cout<<"The first value is: "<<x<<endl;
	//
	//x = ((-b) - (sqrt((b * b) - 4 * a * c))) / (2 * a);
	//cout<<"The second value is: "<<x<<endl;
	//cout<<"That is all"<<endl;

	int array[5] = { 2,3,1,5,4 };
	int i, j, aux;

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 4; j++) {
			if (array[j] > array[j + 1]) {
				aux = array[j];
				array[j] = array[j + 1];
				array[j + 1] = aux;
			}
		}
	}

	cout << "Numeros de forma Ascendente: ";
	for (i = 0; i < 5; i++) {
		cout << array[i] << " ";
	}

	cout << "\nNumeros de forma Descendente: ";
	for (i = 4; i >= 0; i--) {
		cout << array[i] << " ";
	}

	float a;
	cout << "Enter a value" << endl;
	cin >> a;

}

void eleven()
{

	

}

int main()
{
	
	ten_two();

	return 0;

}