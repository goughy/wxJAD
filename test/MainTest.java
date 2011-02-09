
public class MainTest
{
	public static void main( String[] args )
	{
		MainTest mt = new MainTest();
		mt.run( args[ 0 ] );
	}

	private void run( String arg )
	{
		System.out.println( "This is the argument: " + arg );
	}
}
