package mm.android.bookmarksmanager;

// import android.Manifest;
// import android.content.Context;
// import android.content.Intent;
// import android.content.pm.PackageManager;
// import android.os.Build;
// import android.os.Environment;
// import android.provider.Settings;
// import android.view.View;
// import android.widget.Button;
// import android.widget.EditText;
// import android.widget.ListView;
// import android.widget.TextView;
// import androidx.appcompat.app.AppCompatActivity;
// import androidx.core.app.ActivityCompat;
// import androidx.core.content.ContextCompat;
// import java.lang.Runnable;
// import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.widget.Toast;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class BookmarkAdd extends Activity {

    // private EditText titleTxt = null;
    // private EditText urlTxt = null;
    // private Button addBtn = null;
    // private Button cancelBtn = null;
    // private TextView warning = null;
    // private DataAdapter adapter = null;
    // private ListView listView = null;

//    private static native int callNativeOne(int x);

    private static native boolean callInsertBookmark(String title, String url);

    // private void _close()
    // {
    //     this.finish();
    // }

    private void logTxt(String string, boolean toast)
    {
        Log.d("BookmarkAdd", string);
        if (toast)
            Toast.makeText(BookmarkAdd.this, string, Toast.LENGTH_LONG).show();
    }

    private void logTxt(String string)
    {
        logTxt(string, true);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.bookmark_add);
        setFinishOnTouchOutside(false);

        Intent intent = getIntent();

        try
        {
            processIntent(intent);
        }
        catch (Exception e)
        {
            e.printStackTrace();
            System.out.println(e);
            logTxt(e.toString());
        }

        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                BookmarkAdd.this.finish();
            }
        }, 2000);
    }

    private void processIntent(Intent intent)
    {
        if (intent == null || !Intent.ACTION_SEND.equals(intent.getAction()))
        {
            logTxt("Invalid Intent");
            return;
        }

        String extraText = intent.hasExtra(Intent.EXTRA_TEXT) ?
            intent.getStringExtra(Intent.EXTRA_TEXT) : null;
        String extraSubject = intent.hasExtra(Intent.EXTRA_SUBJECT) ?
            intent.getStringExtra(Intent.EXTRA_SUBJECT) : null;

        if (extraText == null)
        {
            logTxt("Invalid URLs");
            return;
        }
        if (extraSubject == null)
        {
            logTxt("Invalid Titles");
            return;
        }

        logTxt("EXTRA_TEXT: " + extraText + " EXTRA_SUBJECT: " + extraSubject, false);

        List<String> tmpUrls = new ArrayList<String>(
                                Arrays.asList(extraText.split("\n")));
        ArrayList<String> urls = new ArrayList<>();

        for (String v : tmpUrls)
        {
            if (v == null)
                continue;
            String _v = v.strip();
            if (_v.length() <= 0)
                continue;
            urls.add(_v);
        }

        if (urls.size() <= 0)
        {
            logTxt("No URL Found");
            return;
        }

        List<String> titles = new ArrayList<String>(Arrays.asList(
                            extraSubject.split(", ", urls.size())));

        logTxt("Titles: " + String.valueOf(titles.size()) + " URLs: " + String.valueOf(urls.size()), false);

        int added = 0;
        int failed = 0;

        for (int i = 0; i < urls.size(); ++i)
        {
            String url = urls.get(i);
            String title = "";
            if (i < titles.size())
                title = titles.get(i);
            boolean res = callInsertBookmark(title, url);
            logTxt("TITLE: " + title + " URL: " + url + " ADDED: " + String.valueOf(res), false);
            if (res)
                added += 1;
            else
                failed += 1;
        }

        logTxt("ADDED: " + String.valueOf(added) + " FAILED: " + String.valueOf(failed));
    }









        // titleTxt = (EditText) findViewById(R.id.titleTxt);
        // urlTxt = (EditText) findViewById(R.id.urlTxt);
        // addBtn = (Button) findViewById(R.id.addBtn);
        // cancelBtn = (Button) findViewById(R.id.cancelBtn);
        // warning = (TextView) findViewById(R.id.warning);

//         addBtn.setOnClickListener(new View.OnClickListener()
//         {
//             @Override
//             public void onClick(View view)
//             {
//                 // int count = listView.getChildCount();
//                 int count = adapter.getCount();
//                 Log.d("BookmarkAdd", "Items Count: " + String.valueOf(count));
//
//                 for (int i = 0; i < count; ++i)
//                 {
//                     DataModel model = adapter.getElement(i);
//
//                     // View childView = listView.getChildAt(i);
//                     // if (childView == null)
//                         // continue;
//                     // EditText titleView = (EditText) childView.findViewById(R.id.title);
//                     // EditText urlView = (EditText) childView.findViewById(R.id.url);
//                     // TextView warning = (TextView) childView.findViewById(R.id.warning);
//                     // String title = titleView.getText().toString();
//                     // String url = urlView.getText().toString();
//                     String title = model.getTitle();
//                     String url = model.getUrl();
//
//                     // String title = titleTxt.getText().toString();
//                     // String url = urlTxt.getText().toString();
//                     if (url == null || url.length() <= 0)
//                     {
//                         // warning.setText("URL is required!");
//                         continue;
//                         // return;
//                     }
//                     if (title == null || title.length() <= 0)
//                     {
//                         // warning.setText("Title is required!");
//                         continue;
//                         // return;
//                     }
//                     Log.d("BookmarkAdd", "Adding [URL]: " + url + " [TITLE]: " + title);
//                     boolean res = callInsertBookmark(title, url);
//                     if (res != true)
//                     {
//                         // warning.setText("Failed to add bookmark!");
//                         continue;
//                         // return;
//                     }
//                     Log.d("BookmarkAdd", "Added [URL]: " + url + " [TITLE]: " + title);
//                     // warning.setText("Added Bookmark");
//                     Toast.makeText(BookmarkAdd.this, "Bookmark Added: " + url, Toast.LENGTH_SHORT).show();
//                     // _close();
//                 }
//
//                 _close();
//
//
// //                String txt = title.getText().toString();
//                 // call native method and expect return
//                 // https://doc.qt.io/qt-6/qjniobject.html
//                 // /opt/user/qt/source/qt-everywhere-src-6.5.1/qtdoc/examples/demos/hangman/purchasing/android/src/org/qtproject/qt/android/purchasing/InAppPurchase.java
//                 // /opt/user/qt/source/qt-everywhere-src-6.5.1/qtdoc/examples/demos/hangman/purchasing/android/androidjni.cpp
// //                int v = callNativeOne(32);
// //                Toast.makeText(BookmarkAdd.this, String.valueOf(v), Toast.LENGTH_SHORT).show();
//             }
//         });

        // cancelBtn.setOnClickListener(new View.OnClickListener()
        // {
        //     @Override
        //     public void onClick(View view)
        //     {
        //         _close();
        //     }
        // });

        // Intent intent = getIntent();
        //
        // if (intent != null && Intent.ACTION_SEND.equals(intent.getAction()))
        // {
        //     String text = intent.hasExtra(Intent.EXTRA_TEXT) ? intent.getStringExtra(Intent.EXTRA_TEXT) : null;
        //     String subject = intent.hasExtra(Intent.EXTRA_SUBJECT) ? intent.getStringExtra(Intent.EXTRA_SUBJECT) : null;
        //
        //     titleTxt.setText(subject);
        //     urlTxt.setText(text);
        // }

        // adapter = new DataAdapter(getApplicationContext(),
        //                             intentToList(getIntent()));
        // listView = (ListView) findViewById(R.id.list);
        // listView.setAdapter(adapter);

        // _setup_views();
        // _process_intent();
    // }

    // public List<DataModel> intentToList(Intent intent)
    // {
    //     ArrayList<DataModel> data = new ArrayList<>();
    //
    //     if (intent == null || !Intent.ACTION_SEND.equals(intent.getAction()))
    //         return data;
    //
    //     String fullText = intent.hasExtra(Intent.EXTRA_TEXT) ?
    //         intent.getStringExtra(Intent.EXTRA_TEXT) : null;
    //     String fullSubject = intent.hasExtra(Intent.EXTRA_SUBJECT) ?
    //         intent.getStringExtra(Intent.EXTRA_SUBJECT) : null;
    //
    //     Log.d("BookmarkAdd-intentToList", "Full [TEXT]: " + fullText + " [SUBJECT]: " + fullSubject);
    //
    //     List<String> urlsTmp = new ArrayList<String>(
    //                             Arrays.asList(fullText.split("\n")));
    //     ArrayList<String> urls = new ArrayList<>();
    //
    //     for (String e : urlsTmp)
    //     {
    //         if (e == null)
    //             continue;
    //         String _e = e.strip();
    //         if (_e.length() <= 0)
    //             continue;
    //         urls.add(_e);
    //     }
    //
    //     if (urls.size() <= 0)
    //         return data;
    //
    //     List<String> titles = new ArrayList<String>(
    //                         Arrays.asList(fullSubject.split(", ", urls.size())));
    //
    //     Log.d("BookmarkAdd-intentToList", "Titles: " + String.valueOf(titles.size()) + " Urls: " + String.valueOf(urls.size()));
    //
    //     for (int i = 0; i < urls.size(); ++i)
    //     {
    //         String u = urls.get(i);
    //         String t = "";
    //         if (i < titles.size())
    //             t = titles.get(i);
    //         Log.d("BookmarkAdd-intentToList", "Single [TITLE]: " + t + " [URL]: " + u);
    //         data.add(new DataModel(t, u));
    //     }
    //
    //     return data;
    // }

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
