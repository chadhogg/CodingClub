package justforfun;

import java.util.Scanner;

/**
 * A solution to https://open.kattis.com/problems/rationalarithmetic.
 * 
 * @author Chad Hogg
 * @version 2021-11-04
 */
public class RationalArithmetic {
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 */
	public static void main(String[] args) {
		@SuppressWarnings("resource")
		Scanner console = new Scanner(System.in);
		int NUM = console.nextInt();
		for(int i = 0; i < NUM; i++) {
			Fraction first = new Fraction(console.nextLong(), console.nextLong());
			String operator = console.next();
			Fraction second = new Fraction(console.nextLong(), console.nextLong());
			switch(operator) {
			case "+":
				System.out.println(Fraction.add(first, second));
				break;
			case "-":
				System.out.println(Fraction.subtract(first, second));
				break;
			case "*":
				System.out.println(Fraction.multiply(first, second));
				break;
			case "/":
				System.out.println(Fraction.divide(first, second));
				break;
			default:
				throw new IllegalStateException("\"" + operator + "\" ain't no operator I've ever seen.");	
			}
		}
	}

	/**
	 * A rational number.
	 * 
	 * @author Chad Hogg
	 */
	private static class Fraction {
		/** The numerator. */
		private long num;
		/** The denominator. */
		private long den;
		
		/** Constructs a new fraction.
		 * 
		 * @param n The numerator of the new fraction.
		 * @param d The denominator of the new fraction.
		 */
		public Fraction(long n, long d) {
			num = n;
			den = d;
			simplify();
		}
		
		/**
		 * Simplifies this fraction as much as possible.
		 */
		private void simplify() {
			long gcd = gcd(Math.abs(num), Math.abs(den));
			num /= gcd;
			den /= gcd;
			if(den < 0) {
				num *= -1;
				den *= -1;
			}
		}
		
		@Override
		public String toString() {
			return num + " / " + den;
		}
		
		/**
		 * Adds two fractions.
		 * 
		 * @param x The first addend.
		 * @param y The second addend.
		 * @return The result, which has been simplified.
		 */
		public static Fraction add(Fraction x, Fraction y) {
			return new Fraction (x.num * y.den + y.num * x.den, x.den * y.den);
		}
		
		/**
		 * Subtracts two fractions.
		 * 
		 * @param x The subtrahend.
		 * @param y The minuend.
		 * @return The result, which has been simplified.
		 */
		public static Fraction subtract(Fraction x, Fraction y) {
			return new Fraction (x.num * y.den - y.num * x.den, x.den * y.den);
		}
		
		/**
		 * Multiplies two fractions.
		 * 
		 * @param x The multiplicand.
		 * @param y The multiplier.
		 * @return The result, which has been simplified.
		 */
		public static Fraction multiply(Fraction x, Fraction y) {
			return new Fraction (x.num * y.num, x.den * y.den);
		}
		
		/**
		 * Divides two fractions.
		 * 
		 * @param x The dividend.
		 * @param y The divisor.
		 * @return The result, which has been simplified.
		 */
		public static Fraction divide(Fraction x, Fraction y) {
			return new Fraction (x.num * y.den, x.den * y.num);
		}
		
		/**
		 * Computes the greatest common divisor of two integers, using the Euclidean algorithm.
		 * 
		 * @param x The first number.
		 * @param y The second number.
		 * @return The largest number that evenly divides both x and y.
		 */
		private static long gcd(long x, long y) {
			if(x == 0) {
				return y;
			}
			else if(y == 0) {
				return x;
			}
			else if(x > y) {
				return gcd(x % y, y);
			}
			else {
				return gcd(x, y % x);
			}
		}
	}
}
