import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletAnalogOutV3;

public class ExampleSimple {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Analog Out Bricklet 3.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletAnalogOutV3 ao = new BrickletAnalogOutV3(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set output voltage to 3.3V
		ao.setOutputVoltage(3300);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
