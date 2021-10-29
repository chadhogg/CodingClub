package fall2021;
import java.util.Scanner;

/**
 * A solution to https://open.kattis.com/problems/mirror.
 * 
 * @author Chad Hogg
 */
public class Mirror {
	/** The character used to display white parts of black and white images. */
	public static final char WHITE_CHAR = '.';
	/** The character used to display black pats of black and white images. */
	public static final char BLACK_CHAR = '*';
	
	/**
	 * A pixel in a black-and-white image.
	 * 
	 * @author Chad Hogg
	 */
	private static enum Pixel {
		/** A black pixel. */
		WHITE(WHITE_CHAR),
		/** A white pixel. */
		BLACK(BLACK_CHAR);
		
		/** The character used to represent this kind of pixel. */
		public final char display;
		
		/**
		 * Constructs a new pixel.
		 * 
		 * @param display The character used to display it.
		 */
		private Pixel(char display) {
			this.display = display;
		}
	}
	
	/**
	 * A black-and-white image.
	 * 
	 * @author Chad Hogg
	 */
	private static class Image {
		/** The bitmap of the image. */
		private Pixel[][] data;
		
		/**
		 * Constructs an image from a textual description.
		 * It must consist of a height, a width, and then an appropriate number of lines containing the characters used for the two pixel types and no spaces.
		 * 
		 * @param input A scanner from which that description can be read.
		 */
		public Image(Scanner input) {
			int height = input.nextInt();
			int width = input.nextInt();
			data = new Pixel[height][width];
			for(int i = 0; i < height; i++) {
				String line = input.next();
				for(int j = 0; j < width; j++) {
					if(line.charAt(j) == WHITE_CHAR) {
						data[i][j] = Pixel.WHITE;
					}
					else if(line.charAt(j) == BLACK_CHAR) {
						data[i][j] = Pixel.BLACK;
					}
					else {
						throw new IllegalArgumentException("Read a character '" + line.charAt(j) + " when expecting either '" + WHITE_CHAR + "' or '" + BLACK_CHAR + "'.");
					}
				}
			}
		}

		/**
		 * Creates a string representing a mirrored picture of this image.
		 * 
		 * @return That string.
		 */
		public String mirroredToString() {
			StringBuilder output = new StringBuilder();
			for(int i = data.length - 1; i >= 0; i--) {
				for(int j = data[i].length - 1; j >= 0; j--) {
					output.append(data[i][j].display);
				}
				output.append("\n");
			}
			return output.toString();
		}
	}
	
	/**
	 * Runs the program.
	 * 
	 * @param args Not used.
	 */
	public static void main(String[] args) {
		Scanner console = new Scanner(System.in);
		int testCount = console.nextInt();
		int testNum = 1;
		while(testNum <= testCount) {
			Image image = new Image(console);
			System.out.println("Test " + testNum);
			System.out.println(image.mirroredToString());
			testNum++;
		}
	}

}
