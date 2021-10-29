package fall2021;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Scanner;

/**
 * A solution to https://open.kattis.com/problems/airconditioned
 * 
 * @author Chad Hogg
 */
public class AirConditioned {
	
	// Notes: It seems relevant that high temperatures are bounded by 2 * number of minions, though I don't yet see why.
	// Possible solution: For each room, keep track of a minimum and maximum, and move those closer to each other as
	//   we add new minions to it.  For a minion who does not fit, create a new room.
	// Problem: Seems like it could easily find a local, not global minima.
	// Possible Solution: Does it help if I always assign the most-constrained minions first?
	// Resolution: That might guarantee optimality, could perhaps be proven by induction, but I'm skeptical of it.
	// Possible solution: Just brute-force it by trying each minion in any room they could go in or a new room.
	// Problem: Might be too slow, but worth a try.
	// Resolution: We'll do that first.
	// Conclusion: That works on the sample test cases, but (not surprisingly) is too slow on larger ones.
	// Resolution: Let's go back to the original idea.
	// Conclusion: Well, it also works on the sample test cases.  So let's see ...
	
	private static class Minion {
		public Minion(int l, int h) {
			minTemp = l;
			maxTemp = h;
		}
		public int minTemp;
		public int maxTemp;
		public boolean canGoInRoom(Room room) {
			return this.minTemp <= room.maxTemp && this.maxTemp >= room.minTemp;
		}
	}
	
	private static class Room {
		public Room(int l, int h) {
			minTemp = l;
			maxTemp = h;
		}
		public int minTemp;
		public int maxTemp;
		public String toString() {
			return "[" + minTemp + ", " + maxTemp + "]";
		}
	}

	public static void main(String[] args) {
		Scanner console = new Scanner(System.in);
		int numMinions = console.nextInt();
		List<Minion> minions = new ArrayList<>();
		while(minions.size() < numMinions) {
			minions.add(new Minion(console.nextInt(), console.nextInt()));
		}
		System.out.println(maybeSmart(minions));
	}
	
	private static int bruteForce(List<Minion> minions) {
		return bruteForce(minions, new ArrayList<Room>());
	}
	
	private static int bruteForce(List<Minion> unassignedMinions, List<Room> rooms) {
		if(unassignedMinions.isEmpty()) {
			return rooms.size();
		}
		int lowest = Integer.MAX_VALUE;
		Minion current = unassignedMinions.get(0);
		List<Minion> others = unassignedMinions.subList(1, unassignedMinions.size());
		for(int i = 0; i < rooms.size(); i++) {
			Room room = rooms.get(i);
			if(current.canGoInRoom(room)) {
				Room replacement = new Room(Math.max(current.minTemp, room.minTemp), Math.min(current.maxTemp, room.maxTemp));
				rooms.set(i, replacement);
				lowest = Math.min(lowest, bruteForce(others, rooms));
				rooms.set(i, room);
			}
		}
		rooms.add(new Room(current.minTemp, current.maxTemp));
		lowest = Math.min(lowest, bruteForce(others, rooms));
		rooms.remove(rooms.size() - 1);
		return lowest;
	}
	
	private static int maybeSmart(List<Minion> minions) {
		minions.sort(new Comparator<Minion>() {
			public int compare(Minion x, Minion y) {
				return (x.maxTemp - x.minTemp) - (y.maxTemp - y.minTemp);
			}
		});
		List<Room> rooms = new ArrayList<>();
		for(int i = 0; i < minions.size(); i++) {
			Minion current = minions.get(i);
			boolean foundARoom = false;
			for(int j = 0; j < rooms.size() && !foundARoom; j++) {
				Room room = rooms.get(j);
				if(current.canGoInRoom(room)) {
					room.minTemp = Math.max(current.minTemp, room.minTemp);
					room.maxTemp = Math.min(current.maxTemp, room.maxTemp);
					foundARoom = true;
				}
			}
			if(!foundARoom) {
				rooms.add(new Room(current.minTemp, current.maxTemp));				
			}
		}
		return rooms.size();
	}
}
