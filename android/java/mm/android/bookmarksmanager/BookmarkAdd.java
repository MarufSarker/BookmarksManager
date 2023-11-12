package mm.android.bookmarksmanager;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.widget.Toast;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class BookmarkAdd extends Activity
{
    private static native boolean callInsertBookmark(String title, String url);

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
}
