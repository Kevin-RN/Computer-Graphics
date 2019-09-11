As you might remember from Calculus or your high school mathematics courses,
complex numbers exist partially in the real world (real), and partially in the
imaginary world (im). In this exercise, you must define operators for complex
numbers such that any essential mathematical operation can be performed. The
class Complex, which is used to store the complex numbers within our program,
is defined as below.

```c
  Class Complex {
   public:
      Complex(float real, float imaginary) {}
      
      float real, im;
  };
```

You are also given a Complexoperation method, in which you must write your
implementation. The second line has been commented out to prevent program
compilation errors. It is given as shown below.

```c
  Complex ComplexOperations(Complex c1, Complex c2) {
    return Complex(0,0);
    // (c1-c2)*(c1+c2);
  }
```

For this assignment, you must implement the addition (+), subtraction (-), and
product (*) operators for complex numbers. Division is not required. Thus, if
your input was (3 + i) + (-1 + 2i), your result would be (2 + 3i). If your input
was (3 + 2i) * (1 + 4i), the result would be (-5 + 14i). You should not change
or remove the real and im variables given, as these are used for the automatic
grading of your code.

Solution:
```c
class Complex {
public:
	Complex(float real, float imaginary){
		this->real = real;
		this->im = imaginary;
	};

	//Method operation for adding (+)
	Complex add(Complex that) {
		return Complex(real + that.real, im + that.im);
	}

	//Method operation for substracting (-)
	Complex sub(Complex that) {
		return Complex(real - that.real, im - that.im);
	}

	//Method operation for multiplying (*)
	Complex mul(Complex that) {
		return Complex((real * that.real) - (that.im * im), (real * that.im) + (im * that.real));
	}

	float real, im;
};
```

Test:
```
  Complex ComplexOperation(Complex c1, Complex c2)
  {
    //return Complex(0, 0);
    //    return (c1-c2) * (c1+c2);

    return (c1.sub(c2)).mul((c1.add(c2)));
  }

  int main {
      Complex c1(1.f, 2.f);
      Complex c2(5.f, -3.f);
      Complex complexAnswer = ComplexOperation(c1, c2);

      if (equalFloats(complexAnswer.real, -19.f) && equalFloats(complexAnswer.im, 34.f))
      {
        std::cout << "Exercise 3: Correct" << std::endl;
      }
      else
      {
        std::cout << "Exercise 3: Incorrect" << std::endl;
      }
   }
  ```
