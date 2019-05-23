public final int PORT = 3;      // Ryan's Macbook pro LHS USB Port

public final int HX_PIN = 0;    // Switch pin numbers
public final int HY_PIN = 1;
public final int HZ_PIN = 2;
public final int EX_PIN = 3;
public final int EY_PIN = 4;
public final int EZ_PIN = 5;

public final boolean NO_HARDWARE_MODE = false;
public boolean IS_MOVING = false;
public boolean CALIBRATING = true;

public final int ROBOT_COLOR = color(0, 130, 126);
public final PVector ROBOT_DRAW_SIZE = new PVector(100, 100);
public final PVector ROBOT_DRAW_LOCATION = new PVector (40, 40);
public final float MM_TO_PIXEL_CONVERSION = 0.0000001;


public final int MOT_DELAY = 2;


public int a2d (int a) {
    if (a < 511) return 0;
    else return 1;
}
