/**
 * 
 */
package com.AmishRobot.LedController;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

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
public class ANI_RAINBOW_Activity extends Activity {

	// Debugging
	private static final String TAG = "ANI_RAINBOW_Activity";
	private static final boolean D = true;
	
	/* Identity of parent activity */
	private LEDControllerActivity PA;
	
	/* View elements */
	private Button mSetButton;
	private SeekBar mRateSeekBar;
	private SeekBar mBrightnessSeekBar;
	private CheckBox mRunningCheckBox;
	private SeekBar mSpeedSeekBar;
	private TextView mSpeedTextView;
	
	
	/* Rainbow parameters */
	private int rate = 75;
	private int brightness = 255;
	private int speed = 18;
	private int isRunning = 0;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		if (D) Log.e(TAG, "+ ON CREATE +");
		
		/* Identify parent activity to use its BT Stack */
		PA = (LEDControllerActivity) this.getParent();		
		
		/* Setup view */
		setContentView(R.layout.tab_rain);
		
		// Initialize the speed layout
		mSpeedTextView = (TextView) findViewById(R.id.tab_rb_txt_speed);
		
		// Initialize rate seek bar with listener
		mRateSeekBar = (SeekBar) findViewById(R.id.tab_rb_seek_rate);
		mRateSeekBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			private int isChanged = 0;
			private int value = 0;

			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				if(isChanged==1) {
					rate = Math.max(0, Math.min(value+1, 255) );
//					setRainbow();
				}
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				isChanged = 0;
				
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				isChanged = 1;
				value = progress;
			}
		});

		// Initialize brightness seek bar with listener
		mBrightnessSeekBar = (SeekBar) findViewById(R.id.tab_rb_seek_brightness);
		mBrightnessSeekBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			private int isChanged = 0;
			private int value = 0;
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				if(isChanged==1) {
					brightness = Math.max(0,  Math.min(value, 255) );
//					setRainbow();
				}
				
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				isChanged = 0;
				
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				isChanged = 1;
				value = progress;
			}
		});
		
		// Initialize running check box with listener
		mRunningCheckBox = (CheckBox) findViewById(R.id.tab_rb_check_running);
		mRunningCheckBox.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
		        // Perform action on clicks, depending on whether it's now checked
		        if (((CheckBox) v).isChecked()) {
		            // Make speed seek visible
		    		mSpeedTextView.setVisibility(View.VISIBLE);
		    		mSpeedSeekBar.setVisibility(View.VISIBLE);
		        	isRunning = 1;
		        } else {
		            // Make speed seek invisible
		    		mSpeedTextView.setVisibility(View.INVISIBLE);
		    		mSpeedSeekBar.setVisibility(View.INVISIBLE);
		        	isRunning = 0;
		        }

				
			}
		});
		
		// Initialize running speed seek with listener
		mSpeedSeekBar = (SeekBar) findViewById(R.id.tab_rb_seek_speed);
		mSpeedSeekBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			private int isChanged = 0;
			private int value = 0;
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				if(isChanged==1) {
					speed = Math.max(0,  Math.min(value, 255) );
//					setRainbow();
				}
				
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				isChanged = 0;
				
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				isChanged = 1;
				value = progress;
			}
		});
		
		// Initialize the Set button with a listener for click events
		mSetButton = (Button) findViewById(R.id.tab_rb_but_set);
		mSetButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				setRainbow();
			}
		});
		
		// Hide running options on create
		mSpeedTextView.setVisibility(View.INVISIBLE); // Hide on create
		mSpeedSeekBar.setVisibility(View.INVISIBLE);
	}
	
	/** Called when the activity is brought to the front */
	@Override
	public void onResume() {
		super.onResume();
		if (D) Log.e(TAG, "+ ON RESUME +");
		
		if(isRunning == 1) {
			mSpeedTextView.setVisibility(View.VISIBLE);
			mSpeedSeekBar.setVisibility(View.VISIBLE);			
		}
		else {
			mSpeedTextView.setVisibility(View.INVISIBLE);
			mSpeedSeekBar.setVisibility(View.INVISIBLE);			
		}
	}	
	
	/** Function to set LEDs as a rainbow */
	private void setRainbow() {
		
		if(isRunning == 1) {
			byte msg[] = new byte[4];
			
			msg[0] = LEDControllerActivity.ANI_RAIN_CHASE;
			msg[1] = (byte) (rate & 0xFF);
			msg[2] = (byte) (brightness & 0xFF);
			msg[3] = (byte) (speed & 0xFF);
			
			PA.sendMessage(msg, 4);
		}
		else {
			byte msg[] = new byte[3];
			
			msg[0] = LEDControllerActivity.ANI_RAINBOW;
			msg[1] = (byte) ( rate & 0xFF );
			msg[2] = (byte) ( brightness & 0xFF );
			
			PA.sendMessage(msg, 3);			
		}
	}
}
