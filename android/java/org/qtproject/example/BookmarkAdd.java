package org.qtproject.example;

import androidx.appcompat.app.AppCompatActivity;

// import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;


public class BookmarkAdd extends AppCompatActivity {

    private EditText title = null;
    private EditText url = null;
    private Button add = null;
    // private Button m_cancel_view = null;
    // private TextView m_warning = null;

    private static native int callNativeOne(int x);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.bookmark_add);
        setFinishOnTouchOutside(false);

        title = (EditText) findViewById(R.id.titleXXX);
        url = (EditText) findViewById(R.id.urlXXX);
        add = (Button) findViewById(R.id.addXXX);

        add.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                String txt = title.getText().toString();
                // call native method and expect return
                // https://doc.qt.io/qt-6/qjniobject.html
                // /opt/user/qt/source/qt-everywhere-src-6.5.1/qtdoc/examples/demos/hangman/purchasing/android/src/org/qtproject/qt/android/purchasing/InAppPurchase.java
                // /opt/user/qt/source/qt-everywhere-src-6.5.1/qtdoc/examples/demos/hangman/purchasing/android/androidjni.cpp
                int v = callNativeOne(32);
                Toast.makeText(BookmarkAdd.this, String.valueOf(v), Toast.LENGTH_SHORT).show();
            }
        });

        Intent intent = getIntent();

        if (intent != null && Intent.ACTION_SEND.equals(intent.getAction()))
        {
            String extra_text = intent.hasExtra(Intent.EXTRA_TEXT) ? intent.getStringExtra(Intent.EXTRA_TEXT) : null;
            String extra_subject = intent.hasExtra(Intent.EXTRA_SUBJECT) ? intent.getStringExtra(Intent.EXTRA_SUBJECT) : null;

            title.setText(extra_subject);
            url.setText(extra_text);
        }

        // _setup_views();
        // _process_intent();
    }

    // private void _close()
    // {
    //     this.finish();
    // }

    // private void _setup_views()
    // {
    //     _find_views();
    //     _attach_callbacks();
    // }

    // private void _find_views()
    // {
    //     m_title_view = findViewById(R.id.title);
    //     m_url_view = findViewById(R.id.url);
    //     m_add_view = findViewById(R.id.add);
    //     m_cancel_view = findViewById(R.id.cancel);
    //     m_warning = findViewById(R.id.warning);
    // }

    // private void _attach_callbacks()
    // {
    //     if (m_cancel_view != null)
    //     {
    //         m_cancel_view.setOnClickListener(new View.OnClickListener() {
    //             @Override
    //             public void onClick(View view) {
    //                 _close();
    //             }
    //         });
    //     }
    //
    //     if (m_add_view != null)
    //     {
    //         m_add_view.setOnClickListener(new View.OnClickListener() {
    //             @Override
    //             public void onClick(View view) {
    //                 String title = m_title_view != null ? m_title_view.getText().toString() : null;
    //                 String url = m_url_view != null ? m_url_view.getText().toString() : null;
    //
    //                 if (url == null || url.length() <= 0)
    //                 {
    //                     m_warning.setText(R.string.url_is_required);
    //                     return;
    //                 }
    //
    //                 if (title == null || title.length() <= 0)
    //                 {
    //                     m_warning.setText(R.string.title_is_required);
    //                     return;
    //                 }
    //
    //                 if (mm_bookmarks.get_instance(getApplicationContext()).bookmark_url_exists(url))
    //                 {
    //                     m_warning.setText(R.string.url_exists);
    //                     return;
    //                 }
    //
    //                 mm_bookmarks.get_instance(getApplicationContext()).insert_bookmark_url(title, url);
    //
    //                 if (!mm_bookmarks.get_instance(getApplicationContext()).bookmark_url_exists(url))
    //                 {
    //                     m_warning.setText(R.string.url_is_not_added);
    //                     return;
    //                 }
    //
    //                 _close();
    //             }
    //         });
    //     }
    // }

    // private void _process_intent()
    // {
    //     Intent intent = getIntent();
    //
    //     if (intent == null)
    //     {
    //         return;
    //     }
    //
    //     if (intent.hasExtra("MM_SELF"))
    //     {
    //         return;
    //     }
    //
    //     if (!Intent.ACTION_SEND.equals(intent.getAction()))
    //     {
    //         if (m_warning != null)
    //         {
    //             m_warning.setText(R.string.unexpected_error);
    //         }
    //         return;
    //     }
    //
    //     String extra_text = null;
    //     String extra_subject = null;
    //
    //     extra_text = intent.hasExtra(Intent.EXTRA_TEXT) ? intent.getStringExtra(Intent.EXTRA_TEXT) : null;
    //     extra_subject = intent.hasExtra(Intent.EXTRA_SUBJECT) ? intent.getStringExtra(Intent.EXTRA_SUBJECT) : null;
    //
    //     if (m_title_view != null)
    //     {
    //         m_title_view.setText(extra_subject);
    //     }
    //
    //     if (m_url_view != null)
    //     {
    //         m_url_view.setText(extra_text);
    //     }
    // }
}
