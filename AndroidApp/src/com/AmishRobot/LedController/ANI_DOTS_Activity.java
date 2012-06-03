/**
 * 
 */
package com.AmishRobot.LedController;

import yuku.ambilwarna.AmbilWarnaKotak;
import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.VerticalSeekBar;

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
public class ANI_DOTS_Activity extends Activity {

	// Debugging
	private static final String TAG = "ANI_DOTS_Activity";
	private static final boolean D = true;
	
	/* Identity of parent activity */
	private LEDControllerActivity PA;
	
	/* View elements */
	private Button mSetButton;
	private CheckBox mRandCheck;
	private SeekBar mBirthrateSeek;
	private SeekBar mLifeSeek;
	
	/* Color picker variables */
	View viewHue;
	AmbilWarnaKotak viewSatVal;
	ImageView viewCursor;
	View viewOldColor;
	View viewNewColor;
	ImageView viewTarget;
	ViewGroup viewContainer;
	float[] currentColorHsv = new float[3];
	
	/* Dots Parameters */
	int color = 0xFFFFFFFF;
	private int prob = 1200;
	private int rate = 2;
	private int isRand = 0;
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		if (D) Log.e(TAG, "+ ON CREATE +");
		
		/* Identify parent activity to use its BT Stack */
		PA = (LEDControllerActivity) this.getParent();
		
		/* Setup view */
		Color.colorToHSV(color, currentColorHsv);
		
		final View view = LayoutInflater.from(getBaseContext()).inflate(R.layout.tab_dots, null);
		viewHue = view.findViewById(R.id.ambilwarna_viewHue);
		viewSatVal = (AmbilWarnaKotak) view.findViewById(R.id.ambilwarna_viewSatBri);
		viewCursor = (ImageView) view.findViewById(R.id.ambilwarna_cursor);
		viewOldColor = view.findViewById(R.id.ambilwarna_warnaLama);
		viewNewColor = view.findViewById(R.id.ambilwarna_warnaBaru);
		viewTarget = (ImageView) view.findViewById(R.id.ambilwarna_target);
		viewContainer = (ViewGroup) view.findViewById(R.id.ambilwarna_viewContainer);

		viewSatVal.setHue(getHue());
		viewOldColor.setBackgroundColor(color);
		viewNewColor.setBackgroundColor(color);
		
		// Initialize the Set button with a listener for click events
		mSetButton = (Button) view.findViewById(R.id.tab_dots_but_set);
		mSetButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				
				if(isRand == 1) {
					byte msg[] = new byte[7];
					
					msg[0] = LEDControllerActivity.ANI_DOTS;
					msg[1] = (byte) (((prob & 0xFF00) >> 8) & 0xFF);
					msg[2] = (byte) (  prob                 & 0xFF);
					msg[3] = (byte) (rate & 0xFF);
					msg[4] = (byte) (0 & 0xFF);
					msg[5] = (byte) (0 & 0xFF);
					msg[6] = (byte) (0 & 0xFF);
					
					if (D) Log.e(TAG, "Sent rate = " + rate + "; prob = " + prob);
					
					PA.sendMessage(msg, 7);
				}
				else {
					getColor();

					byte msg[] = new byte[7];
					
					msg[0] = LEDControllerActivity.ANI_DOTS;
					msg[1] = (byte) ((prob >> 8) & 0xFF);
					msg[2] = (byte) (prob & 0xFF);
					msg[3] = (byte) (rate & 0xFF);
					msg[4] = (byte) (( ( (color & 0x00FF0000) >>16) / 8) & 0xFF);
					msg[5] = (byte) (( ( (color & 0x0000FF00) >> 8) / 8) & 0xFF);
					msg[6] = (byte) (( ( (color & 0x000000FF)     ) / 8) & 0xFF);
					
					PA.sendMessage(msg, 7);
					
					viewOldColor.setBackgroundColor(color);
				}

				
			}
		});
		
		// Initialize isRand check box with listener
		mRandCheck = (CheckBox) view.findViewById(R.id.tab_dots_check_rand);
		mRandCheck.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
		        // Perform action on clicks, depending on whether it's now checked
		        if (((CheckBox) v).isChecked()) {
		            // Make color picker visible
		        	viewContainer.setVisibility(View.INVISIBLE);
		        	isRand = 1;
		        } else {
		            // Make color picker invisible
		        	viewContainer.setVisibility(View.VISIBLE);
		        	isRand = 0;
		        }

				
			}
		});
		
		// Initialize mBirthrateSeek seek bar with listener
		mBirthrateSeek = (SeekBar) view.findViewById(R.id.tab_dots_seek_brithrate);
		mBirthrateSeek.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			private int isChanged = 0;
			private int value = 0;
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				if(isChanged==1) {
					prob = value;

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
		
		// Initialize mLifeSeek seek bar with listener		
		mLifeSeek = (VerticalSeekBar) view.findViewById(R.id.tab_dots_seek_lifespan);
		mLifeSeek.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			// For some reason onStopTrackingTouch doesn't work with verticalSeekBar
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				rate = mLifeSeek.getProgress();
				
			}
		});
		
		/* Listener for Hue change */
		viewHue.setOnTouchListener(new View.OnTouchListener() {
			@Override public boolean onTouch(View v, MotionEvent event) {
				if (event.getAction() == MotionEvent.ACTION_MOVE
						|| event.getAction() == MotionEvent.ACTION_DOWN
						|| event.getAction() == MotionEvent.ACTION_UP) {

					float y = event.getY();
					if (y < 0.f) y = 0.f;
					if (y > viewHue.getMeasuredHeight()) y = viewHue.getMeasuredHeight() - 0.001f; // to avoid looping from end to start.
					float hue = 360.f - 360.f / viewHue.getMeasuredHeight() * y;
					if (hue == 360.f) hue = 0.f;
					setHue(hue);

					// update view
					viewSatVal.setHue(getHue());
					moveCursor();
					viewNewColor.setBackgroundColor(getColor());

					return true;
				}
				return false;
			}
		});
		
		/* Listener for saturation value change */
		viewSatVal.setOnTouchListener(new View.OnTouchListener() {
			@Override public boolean onTouch(View v, MotionEvent event) {
				if (event.getAction() == MotionEvent.ACTION_MOVE
						|| event.getAction() == MotionEvent.ACTION_DOWN
						|| event.getAction() == MotionEvent.ACTION_UP) {

					float x = event.getX(); // touch event are in dp units.
					float y = event.getY();

					if (x < 0.f) x = 0.f;
					if (x > viewSatVal.getMeasuredWidth()) x = viewSatVal.getMeasuredWidth();
					if (y < 0.f) y = 0.f;
					if (y > viewSatVal.getMeasuredHeight()) y = viewSatVal.getMeasuredHeight();

					setSat(1.f / viewSatVal.getMeasuredWidth() * x);
					setVal(1.f - (1.f / viewSatVal.getMeasuredHeight() * y));

					// update view
					moveTarget();
					viewNewColor.setBackgroundColor(getColor());

					return true;
				}
				return false;
			}
		});
		
		// Set up the window layout
		setContentView(view);
		
		// move cursor & target on first draw
		ViewTreeObserver vto = view.getViewTreeObserver();
		vto.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
			@Override public void onGlobalLayout() {
				moveCursor();
				moveTarget();
				view.getViewTreeObserver().removeGlobalOnLayoutListener(this);
			}
		});
	}

	protected void moveCursor() {
		float y = viewHue.getMeasuredHeight() - (getHue() * viewHue.getMeasuredHeight() / 360.f);
		if (y == viewHue.getMeasuredHeight()) y = 0.f;
		RelativeLayout.LayoutParams layoutParams = (RelativeLayout.LayoutParams) viewCursor.getLayoutParams();
		layoutParams.leftMargin = (int) (viewHue.getLeft() - Math.floor(viewCursor.getMeasuredWidth() / 2) - viewContainer.getPaddingLeft());
		;
		layoutParams.topMargin = (int) (viewHue.getTop() + y - Math.floor(viewCursor.getMeasuredHeight() / 2) - viewContainer.getPaddingTop());
		;
		viewCursor.setLayoutParams(layoutParams);
	}

	protected void moveTarget() {
		float x = getSat() * viewSatVal.getMeasuredWidth();
		float y = (1.f - getVal()) * viewSatVal.getMeasuredHeight();
		RelativeLayout.LayoutParams layoutParams = (RelativeLayout.LayoutParams) viewTarget.getLayoutParams();
		layoutParams.leftMargin = (int) (viewSatVal.getLeft() + x - Math.floor(viewTarget.getMeasuredWidth() / 2) - viewContainer.getPaddingLeft());
		layoutParams.topMargin = (int) (viewSatVal.getTop() + y - Math.floor(viewTarget.getMeasuredHeight() / 2) - viewContainer.getPaddingTop());
		viewTarget.setLayoutParams(layoutParams);
	}

	private int getColor() {
		color = Color.HSVToColor(currentColorHsv);
		return color;
	}

	private float getHue() {
		return currentColorHsv[0];
	}

	private float getSat() {
		return currentColorHsv[1];
	}

	private float getVal() {
		return currentColorHsv[2];
	}

	private void setHue(float hue) {
		currentColorHsv[0] = hue;
	}

	private void setSat(float sat) {
		currentColorHsv[1] = sat;
	}

	private void setVal(float val) {
		currentColorHsv[2] = val;
	}
}
