package fall2021;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * A solution to https://open.kattis.com/problems/safehouses
 * 
 * @author Chad Hogg
 */
public class SafeHouses {

	private static enum Location {
		SPY('S'),
		HOUSE('H'),
		NOTHING('.');
		
		public final char symbol;
		
		private Location(char s) {
			symbol = s;
		}
		
		public static Location fromSymbol(char s) {
			for(Location loc : values()) {
				if(loc.symbol == s) {
					return loc;
				}
			}
			throw new IllegalArgumentException("No such location type.");
		}
	}
	
	private static class Problem {
		public final int N;
		public final Location[][] locs;
		
		public Problem(Scanner input) {
			N = input.nextInt();
			input.nextLine(); // clear newline from buffer
			locs = new Location[N][N];
			for(int i = 0; i < N; i++) {
				String line = input.nextLine();
				for(int j = 0; j < N; j++) {
					locs[i][j] = Location.fromSymbol(line.charAt(j));
				}
			}
		}
		
		@Override
		public String toString() {
			StringBuffer buf = new StringBuffer();
			buf.append(N).append("\n");
			for(int i = 0; i < N; i++) {
				for(int j = 0; j < N; j++) {
					buf.append(locs[i][j].symbol);
				}
				buf.append("\n");
			}
			return buf.toString();
		}
	}
	
	private static class Coordinate {
		public final int row;
		public final int col;
		public Coordinate(int r, int c) {
			row = r;
			col = c;
		}
		@Override
		public boolean equals(Object obj) {
			if(obj == null) { return false; }
			if(obj.getClass() != this.getClass()) { return false; }
			Coordinate other = (Coordinate)obj;
			return (row == other.row && col == other.col);
		}
		@Override
		public int hashCode() {
			return row + col * 12345;
		}
	}
	
	public static void main(String[] args) {
		Scanner console = new Scanner(System.in);
		Problem prob = new Problem(console);
		System.out.println(findLongestDistance(prob));
	}
	
	private static int findLongestDistance(Problem prob) {
		List<Coordinate> houses = getSafeHouseCoordinates(prob);
		int longest = 0;
		for(int i = 0; i < prob.N; i++) {
			for(int j = 0; j < prob.N; j++) {
				if(prob.locs[i][j] == Location.SPY) {
					int dist = distanceForSpy(i, j, houses);
					if(dist > longest) {
						longest = dist;
					}
				}
			}
		}
		return longest;
	}
	
	private static int distanceForSpy(int i, int j, List<Coordinate> houses) {
		int bestSoFar = Integer.MAX_VALUE;
		for(Coordinate house : houses) {
			int dist = dist(house, i, j);
			if(dist < bestSoFar) {
				bestSoFar = dist;
			}
		}
		return bestSoFar;
	}
	
	private static List<Coordinate> getSafeHouseCoordinates(Problem prob) {
		List<Coordinate> coords = new ArrayList<>();
		for(int i = 0; i < prob.N; i++) {
			for(int j = 0; j < prob.N; j++) {
				if(prob.locs[i][j] == Location.HOUSE) {
					coords.add(new Coordinate(i, j));
				}
			}
		}
		return coords;
	}
	
	private static int dist(Coordinate house, int x, int y) {
		return Math.abs(house.row - x) + Math.abs(house.col - y);
	}
}
