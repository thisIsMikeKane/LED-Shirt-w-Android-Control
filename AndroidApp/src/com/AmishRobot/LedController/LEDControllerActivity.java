package com.AmishRobot.LedController;

import android.app.Activity;
import android.app.TabActivity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.apps.iosched.ui.ScheduleActivity.FlingableTabHost;
/**
 * @author Mike Kane
 *     This file is part of LED-Shirt-w-Android-Control.
 *     
 *     Copyright (C) 2011 Michael Kane
 *     https://github.com/thisIsMikeKane/LED-Shirt-w-Android-Control#led-shirt-w-android-control
 *     
 *     LED-Shirt-w-Android-Control is free software: you can redistribute it 
 *     and/or modify it under the terms of the GNU General Public License as 
 *     published by the Free Software Foundation, either version 3 of the 
 *     License, or (at your option) any later version.
 *     
 *     LED-Shirt-w-Android-Control is distributed in the hope that it will 
 *     be useful, but WITHOUT ANY WARRANTY; without even the implied warranty 
 *     of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *     
 *     You should have received a copy of the GNU General Public License
 *     along with LED-Shirt-w-Android-Control.  
 *     If not, see <http://www.gnu.org/licenses/>.
 */
public class LEDControllerActivity extends TabActivity {

	// Debugging
	private static final String TAG = "LedCtrlr";
	private static final boolean D = true;

	// Message types sent from the BluetoothChatService Handler
	public static final int MESSAGE_STATE_CHANGE 	= 1;
	public static final int MESSAGE_READ 			= 2;
	public static final int MESSAGE_WRITE 			= 3;
	public static final int MESSAGE_DEVICE_NAME 	= 4;
	public static final int MESSAGE_TOAST 			= 5;

	// Key names received from the BluetoothChatService Handler
	public static final String DEVICE_NAME = "device_name";
	public static final String TOAST = "toast";

	// Intent request codes
	private static final int REQUEST_CONNECT_DEVICE_SECURE 		= 1;
	private static final int REQUEST_CONNECT_DEVICE_INSECURE 	= 2;
	private static final int REQUEST_ENABLE_BT 					= 3;

	// Layout Views	
	static final int COLOR_PICKER_DIALOG_ID = 0;

	// Name of the connected device
	private String mConnectedDeviceName = null;
	// String buffer for outgoing messages
	private StringBuffer mOutStringBuffer;
	// Local Bluetooth adapter
	private BluetoothAdapter mBluetoothAdapter = null;
	// Member object for the chat services
	private BluetoothChatService mChatService = null;
		
	// Message variables
	private int check_sum = 0; 

	/* Animation Defines */
	public static final byte SET_ALL_RGB =		1;
	public static final byte ANI_STROBE =    	2;
	public static final byte ANI_FADE =      	3;
	public static final byte ANI_RAINBOW =   	4;
	public static final byte ANI_RAIN_CHASE =  	5;
	public static final byte ANI_DOTS = 		6;
	public static final byte ANI_DOTS_FLASH = 	7;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		if (D)Log.e(TAG, "+++ ON CREATE +++");
		
		// Set up the window layout
		setContentView(R.layout.main);
	
		// Get local Bluetooth adapter
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

		// If the adapter is null, then Bluetooth is not supported
		if (mBluetoothAdapter == null) {
			Toast.makeText(this, "Bluetooth is not available",
					Toast.LENGTH_LONG).show();
			finish();
			return;
		}

	    Resources res = getResources(); // Resource object to get Drawables
	    FlingableTabHost tabHost = (FlingableTabHost) getTabHost();  // The activity TabHost
	    FlingableTabHost.TabSpec spec;  // Reusable TabSpec for each tab
	    Intent intent;  // Reusable Intent for each tab	    

	    // Initialize a TabSpec for the RGB controller
	    intent = new Intent().setClass(this, SET_ALL_RGB_Activity.class);
	    spec = tabHost.newTabSpec("set_rgb").setIndicator("RGB",
	                      res.getDrawable(R.drawable.ic_tab_rgb))
	                  .setContent(intent);
	    tabHost.addTab(spec);
	    
	    // Initialize a TabSpec for rainbow controller
	    intent = new Intent().setClass(this, ANI_RAINBOW_Activity.class);
	    spec = tabHost.newTabSpec("rainbow").setIndicator("Rainbow",
				res.getDrawable(R.drawable.ic_tab_rain))
				   .setContent(intent);
	    tabHost.addTab(spec);	   

	    // Initialize a TabSpec for the dots controller
	    intent = new Intent().setClass(this, ANI_DOTS_Activity.class);
	    spec = tabHost.newTabSpec("dots").setIndicator("Dots", 
				res.getDrawable(R.drawable.ic_tab_dots))
				   .setContent(intent);
	    tabHost.addTab(spec);

	    tabHost.setCurrentTab(0);

	}

	@Override
	public void onStart() {
		super.onStart();
		if (D)
			Log.e(TAG, "++ ON START ++");

		// If BT is not on, request that it be enabled.
		// setupChat() will then be called during onActivityResult
		if (!mBluetoothAdapter.isEnabled()) {
			Intent enableIntent = new Intent(
					BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
			// Otherwise, setup the chat session
		} else {
			if (mChatService == null)
				setupChat();
		}
	}

	@Override
	public synchronized void onResume() {
		super.onResume();
		if (D) Log.e(TAG, "+ ON RESUME +");

		// Performing this check in onResume() covers the case in which BT was
		// not enabled during onStart(), so we were paused to enable it...
		// onResume() will be called when ACTION_REQUEST_ENABLE activity
		// returns.
		if (mChatService != null) {
			// Only if the state is STATE_NONE, do we know that we haven't
			// started already
			if (mChatService.getState() == BluetoothChatService.STATE_NONE) {
				// Start the Bluetooth chat services
				mChatService.start();
			}
		}
	}

	private void setupChat() {
		Log.d(TAG, "setupChat()");
			
		// Initialize the BluetoothChatService to perform bluetooth connections
		mChatService = new BluetoothChatService(this, mHandler);
				
		// Initialize the buffer for outgoing messages
		mOutStringBuffer = new StringBuffer("");
	}

	@Override
	public synchronized void onPause() {
		super.onPause();
		if (D)
			Log.e(TAG, "- ON PAUSE -");
	}

	@Override
	public void onStop() {
		super.onStop();
		if (D)
			Log.e(TAG, "-- ON STOP --");
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		// Stop the Bluetooth chat services
		if (mChatService != null)
			mChatService.stop();
		if (D)
			Log.e(TAG, "--- ON DESTROY ---");
	}
	
	public void sendMessage(byte message[], int nBytes) {
		// Check that we're actually connected before trying anything
		if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
			Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT)
					.show();
			return;
		}
		
		// Check that there's actually something to send
		if(nBytes > 0) {
			byte[] sendFIFO = new byte[nBytes + 11];
			
			/* Write pre-amble to FIFO */
			sendFIFO[0]=' ';sendFIFO[1]='>';sendFIFO[2]='>';sendFIFO[3]='>';sendFIFO[4]=' ';
			
			/* Write message to FIFO */
			for(int i=0;i<nBytes;i++) {
				sendFIFO[5 + i] = message[i];
			}
			
			/* Write length byte to FIFO */
			sendFIFO[5 + nBytes] = (byte) (nBytes + 11);
			
			/* Write message terminator */
			sendFIFO[nBytes+6]=' ';sendFIFO[nBytes+7]='!';sendFIFO[nBytes+8]='!';sendFIFO[nBytes+9]='!';sendFIFO[nBytes+10]=' ';
			
			/* Send message */
			if (D)Log.d(TAG, "BTSend:" + byteArrayToHexString(sendFIFO) );
			mChatService.write(sendFIFO);
			
			/* Compute check sum */
			check_sum = 0;
			for(int i=0;i<nBytes+11;i++)
			{
				/* Deal with signed/unsigned bull shit */
				check_sum += (int)sendFIFO[i] & 0xFF;
			}
			if (D)Log.d(TAG, "Check Sum = " + check_sum);
			
			// Reset out string buffer to zero and clear the edit text field
			mOutStringBuffer.setLength(0);
		}
	}

	private final void setStatus(int resId) {
		if (D)
			Log.e(TAG, "+ UPDATE CONNECTION STATUS +");
		TextView mTextView = (TextView) findViewById(R.id.txt_status);

		mTextView.setText(resId);
	}

	private final void setStatus(CharSequence subTitle) {
		if (D)
			Log.e(TAG, "+ UPDATE CONNECTION STATUS +");
		TextView mTextView = (TextView) findViewById(R.id.txt_status);

		mTextView.setText(subTitle);
	}

	// The Handler that gets information back from the BluetoothChatService
	private final Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MESSAGE_STATE_CHANGE:
				if (D)
					Log.i(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
				switch (msg.arg1) {
				case BluetoothChatService.STATE_CONNECTED:
					setStatus(getString(R.string.title_connected_to,
							mConnectedDeviceName));
					break;
				case BluetoothChatService.STATE_CONNECTING:
					setStatus(R.string.title_connecting);
					break;
				case BluetoothChatService.STATE_LISTEN:
				case BluetoothChatService.STATE_NONE:
					setStatus(R.string.title_not_connected);
					break;
				}
				break;
			case MESSAGE_WRITE:

				break;
				
	        case MESSAGE_READ:

	        	break;
				
			case MESSAGE_DEVICE_NAME:
				// save the connected device's name
				mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
				Toast.makeText(getApplicationContext(),
						"Connected to " + mConnectedDeviceName,
						Toast.LENGTH_SHORT).show();
				break;
			case MESSAGE_TOAST:
				Toast.makeText(getApplicationContext(),
						msg.getData().getString(TOAST), Toast.LENGTH_SHORT)
						.show();
				break;
			}
		}
	};

	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (D)
			Log.d(TAG, "onActivityResult " + resultCode);
		switch (requestCode) {
		case REQUEST_CONNECT_DEVICE_SECURE:
			// When DeviceListActivity returns with a device to connect
			if (resultCode == Activity.RESULT_OK) {
				connectDevice(data, true);
			}
			break;
		case REQUEST_CONNECT_DEVICE_INSECURE:
			// When DeviceListActivity returns with a device to connect
			if (resultCode == Activity.RESULT_OK) {
				connectDevice(data, false);
			}
			break;
		case REQUEST_ENABLE_BT:
			// When the request to enable Bluetooth returns
			if (resultCode == Activity.RESULT_OK) {
				// Bluetooth is now enabled, so set up a chat session
				setupChat();
			} else {
				// User did not enable Bluetooth or an error occurred
				Log.d(TAG, "BT not enabled");
				Toast.makeText(this, R.string.bt_not_enabled_leaving,
						Toast.LENGTH_SHORT).show();
				finish();
			}
		}
	}

	private void connectDevice(Intent data, boolean secure) {
		// Get the device MAC address
		String address = data.getExtras().getString(
				DeviceListActivity.EXTRA_DEVICE_ADDRESS);
		// Get the BluetoothDevice object
		BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
		// Attempt to connect to the device
		mChatService.connect(device, secure);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		if (D)
			Log.i(TAG, "onCreateOptionsMenu");
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.layout.option_menu, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Intent serverIntent = null;
		switch (item.getItemId()) {
		case R.id.insecure_connect_scan:
			if (D)
				Log.i(TAG, "REQUEST_CONNECT_DEVICE_INSECURE");
			// Launch the DeviceListActivity to see devices and do scan
			serverIntent = new Intent(this, DeviceListActivity.class);
			startActivityForResult(serverIntent,
					REQUEST_CONNECT_DEVICE_INSECURE);
			return true;
		}
		return false;
	}
	
	public static String byteArrayToHexString(byte[] b) {
		StringBuffer sb = new StringBuffer(b.length * 2);
		for (int i = 0; i < b.length; i++) {
			int v = b[i] & 0xff;
			if (v < 16) {
				sb.append('0');
			}
			sb.append(Integer.toHexString(v));
			sb.append(' ');
		}
		return sb.toString().toUpperCase();
	}
	
	public static String byteArrayToHexString(short[] b) {
		StringBuffer sb = new StringBuffer(b.length * 2);
		for (int i = 0; i < b.length; i++) {
			int v = b[i] & 0xff;
			if (v < 16) {
				sb.append('0');
			}
			sb.append(Integer.toHexString(v));
			sb.append(' ');
		}
		return sb.toString().toUpperCase();
	}	
}
