package com.example.yong.test5;

import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import org.deeplearning4j.nn.conf.NeuralNetConfiguration;
import org.deeplearning4j.nn.conf.layers.DenseLayer;
import org.deeplearning4j.nn.conf.layers.OutputLayer;
import org.deeplearning4j.nn.multilayer.MultiLayerNetwork;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.dataset.DataSet;
import org.nd4j.linalg.factory.Nd4j;

import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {
    public static final String EXTRA_MESSAGE = "com.example.myfirstapp.MESSAGE";
    public int g_interations;
    public float g_learningRate;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    /** Called when the user taps the Send button */
    public void sendMessage(View view) {
        //Intent intent = new Intent(this, DisplayMessageActivity.class);
        EditText iteraionsText = (EditText) findViewById(R.id.iterations);
        EditText learningRateText = (EditText) findViewById(R.id.learningRate);

        String iteraions = iteraionsText.getText().toString();
        String learningRate = learningRateText.getText().toString();
        TextView textView = findViewById(R.id.textView);

        if(iteraions.equals(null) || iteraions.equals("") || iteraions.equals(" ")){
            g_interations = 0;
        } else {
            g_interations = Integer.valueOf(iteraions);
        }

        if(learningRate.equals(null) || learningRate.equals("") || learningRate.equals(" ")){
            g_learningRate = 0;
        } else {
            g_learningRate = Float.valueOf(learningRate);
        }

        if (g_interations > 0 && g_interations < 10 * 1000 && g_learningRate > 0 && g_learningRate < 1.0) {
            textView.setText("Conducting training and DL...");
            try{Thread.sleep(500);}catch(InterruptedException e){System.out.println(e);}

            createAndUseNetwork(g_interations, g_learningRate, textView);
        } else {
            textView.setText("The value is out of range!!");
        }

        //String message = "Iterations ("+iteraions + ")    Learning Rate (" + learningRate + ")";
        //Log.d("myTag", message);

        //intent.putExtra(EXTRA_MESSAGE, message);
        //startActivity(intent);
    }

    private void createAndUseNetwork(int interations, float learningRate, TextView textView) {
        DenseLayer inputLayer = new DenseLayer.Builder()
                .nIn(2)
                .nOut(3)
                .name("Input")
                .build();

        DenseLayer hiddenLayer = new DenseLayer.Builder()
                .nIn(3)
                .nOut(2)
                .name("Hidden")
                .build();

        OutputLayer outputLayer = new OutputLayer.Builder()
                .nIn(2)
                .nOut(1)
                .name("Output")
                .build();


        NeuralNetConfiguration.Builder nncBuilder = new NeuralNetConfiguration.Builder();
        nncBuilder.iterations(interations);
        nncBuilder.learningRate(learningRate);

        NeuralNetConfiguration.ListBuilder listBuilder = nncBuilder.list();
        listBuilder.layer(0, inputLayer);
        listBuilder.layer(1, hiddenLayer);
        listBuilder.layer(2, outputLayer);

        listBuilder.backprop(true);

        MultiLayerNetwork myNetwork = new MultiLayerNetwork(listBuilder.build());
        myNetwork.init();

        final int NUM_SAMPLES = 4;

        INDArray trainingInputs = Nd4j.zeros(NUM_SAMPLES, inputLayer.getNIn());
        INDArray trainingOutputs = Nd4j.zeros(NUM_SAMPLES, outputLayer.getNOut());

        // If 0,0 show 0
        trainingInputs.putScalar(new int[]{0,0}, 0);
        trainingInputs.putScalar(new int[]{0,1}, 0);
        trainingOutputs.putScalar(new int[]{0,0}, 0);

        // If 0,1 show 1
        trainingInputs.putScalar(new int[]{1,0}, 0);
        trainingInputs.putScalar(new int[]{1,1}, 1);
        trainingOutputs.putScalar(new int[]{1,0}, 1);

        // If 1,0 show 1
        trainingInputs.putScalar(new int[]{2,0}, 1);
        trainingInputs.putScalar(new int[]{2,1}, 0);
        trainingOutputs.putScalar(new int[]{2,0}, 1);

        // If 1,1 show 0
        trainingInputs.putScalar(new int[]{3,0}, 1);
        trainingInputs.putScalar(new int[]{3,1}, 1);
        trainingOutputs.putScalar(new int[]{3,0}, 0);

        textView.setText("Training is ongoing...");

        DataSet myData = new DataSet(trainingInputs, trainingOutputs);
        myNetwork.fit(myData);

        textView.setText("Training is done...");

        // Create input
        INDArray actualInput = Nd4j.zeros(1,2);
        actualInput.putScalar(new int[]{0,0}, 1);
        actualInput.putScalar(new int[]{0,1}, 1);

        // Generate output
        INDArray actualOutput = myNetwork.output(actualInput);
        Log.d("myNetwork Output", actualOutput.toString());

        textView.setText("Output accuracy: " + actualOutput.toString());
    }
}
