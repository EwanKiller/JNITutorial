package com.ewan.exportlib;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.widget.TextView;

import com.ewan.exportlib.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    static final String TAG = "Ewan";

    static final int[] intArray = { 1, 2, 3 };

    // Used to load the 'exportlib' library on application startup.
    static {
        System.loadLibrary("exportlib");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        //-----------------------

        Log.v(TAG, "[Java] Java call C++, no return , no param");
        initialize();

        Log.v(TAG, "[Java] Java call C++, no return , pass int array");
        passIntArray(intArray);

        Log.v(TAG, "[Java] Java call C++, return float array , no param");
        float[] returnFloatArray = returnFloatArray();
        Log.v(TAG,"[Java] return float array :" + returnFloatArray[0] + "," + returnFloatArray[1] + "," + returnFloatArray[2]);

        Log.v(TAG, "[Java] Java call C++ to drive C++ call Java func");
        callNative();
    }

    /**
     * A native method that is implemented by the 'exportlib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * 1.声明native method : 没有返回值，无参数
     * */
    public native void initialize();

    /**
     * 2.声明native method : 没有返回值，参数为 int数组
     * @param intArray
     */
    public native void passIntArray(int[] intArray);

    /**
     * 3.声明native method : 返回值为float数组，无参数
     * @return
     */
    public native float[] returnFloatArray();

    /**
     * 调一个native func来执行c++里的函数,让C++ call Java func
     */
    public native void callNative();

    /**
     * 4.called by c++ : 无返回值，无参数
     */
    public void Initialize() {
        Log.v(TAG, "[Java]C++ call Java , no return, no param");
    }

    /**
     * 5. called by c++ : 无返回值，参数为string
     * @param funcName
     */
    public void PassStringValue(String funcName) {
        Log.v(TAG, "[Java]C++ call Java , no return, pass string: " + funcName);
    }

    /**
     * 6. called bt c++ : 返回值为string， 无参数
     * @return
     */
    public String ReturnString() {
        String str = "Hello c++";
        Log.v(TAG, "[Java]C++ call Java , return string: " + str +", no param");
        return str;
    }
}