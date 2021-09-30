import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;
import java.util.SortedSet;
import java.util.TreeSet;

/**
 * A solution to https://open.kattis.com/problems/bank.
 * 
 * @author Chad Hogg
 */
public class Bank {

	// Note: It seems to me that there's no downside to scheduling a person as late as they can go, so we can commit to that.
	// Thus, if we schedule the highest value people first, as far in the future as they can go, I think we'll get an optimal solution.
	// If two people are tied for value, we will schedule the one that is less constrained first.
	
	private static class Patron implements Comparable<Patron> {
		public final int dollarValue;
		public final int leaveTime;
		public Patron(int dollarValue, int leaveTime) {
			this.dollarValue = dollarValue;
			this.leaveTime = leaveTime;
		}
		@Override
		public boolean equals(Object obj) {
			if(obj == null) {
				return false;
			}
			if(obj.getClass() != this.getClass()) {
				return false;
			}
			Patron other = (Patron)obj;
			return dollarValue == other.dollarValue && leaveTime == other.leaveTime;
		}
		@Override
		public int compareTo(Bank.Patron o) {
			if(dollarValue == o.dollarValue) {
				return o.leaveTime - leaveTime;
			}
			else {
				return o.dollarValue - dollarValue;
			}
		}
		@Override
		public String toString() {
			return "$" + dollarValue + " leaving at " + leaveTime;
		}
	}
	
	public static void main(String[] args) {
		Scanner console = new Scanner(System.in);
		int numPeople = console.nextInt();
		int numMinutes = console.nextInt();
		SortedSet<Patron> patrons = new TreeSet<>();
		for(int counter = 0; counter < numPeople; counter++) {
			patrons.add(new Patron(console.nextInt(), console.nextInt()));
		}
		Patron[] schedule = new Patron[numMinutes];
		for(Patron patron : patrons) {
			addPatronIfPossible(patron, schedule);
		}
		int total = 0;
		for(Patron patron : schedule) {
			if(patron != null) {
				total += patron.dollarValue;
			}
		}
		System.out.println(total);
	}
	
	private static void addPatronIfPossible(Patron patron, Patron[] schedule) {
		for(int index = patron.leaveTime; index >= 0; index--) {
			if(schedule[index] == null) {
				//System.out.println("Scheduling " + patron + " at time " + index);
				schedule[index] = patron;
				return;
			}
		}
	}
}
