package com.example.computernetwork

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.core.content.ContextCompat
import com.example.computernetwork.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(R.layout.activity_main)
    }

    private fun changeToRedColor() {
        binding.stateTextView.text = "경고"
    }

    private fun changeToYellowColor() {
        binding.stateTextView.text = "관심"
    }
}