package org.muffin.imgui;

import android.os.Bundle;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.initialization.InitializationStatus;
import com.google.android.gms.ads.initialization.OnInitializationCompleteListener;

import org.muffin.imgui.databinding.ActivityMainBinding;
import org.muffin.imgui.muffin.Muffin;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Muffin.Start(this);

        ActivityMainBinding binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        MobileAds.initialize(this, initializationStatus -> {
        });
    }

    /**
     * A native method that is implemented by the 'imgui' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}