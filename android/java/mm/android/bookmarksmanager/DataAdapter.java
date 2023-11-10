// package mm.android.bookmarksmanager;
//
// import android.widget.ArrayAdapter;
// import android.content.Context;
// import android.content.Intent;
// import android.view.View;
// import android.view.ViewGroup;
// import android.view.LayoutInflater;
// import android.widget.EditText;
// import android.util.Log;
// import android.text.TextWatcher;
// import android.text.Editable;
// import java.lang.CharSequence;
//
// import java.util.ArrayList;
// import java.util.Arrays;
// import java.util.List;
//
//
// public class DataAdapter extends ArrayAdapter<DataModel>
// {
//     private List<DataModel> dataModels;
//     private Context context;
//
//     public DataAdapter(Context _context, List<DataModel> objects)
//     {
//         super(_context, -1, objects);
//         this.dataModels = objects;
//         this.context = _context;
//     }
//
//     @Override
//     public int getCount()
//     {
//         return dataModels.size();
//     }
//
//     public DataModel getElement(int pos)
//     {
//         return dataModels.get(pos);
//     }
//
//     @Override
//     public View getView(int position, View convertView, ViewGroup parent)
//     {
//         final View view;
//         if (convertView == null)
//         {
//             LayoutInflater inflater = (LayoutInflater)
//                         context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
//             view = inflater.inflate(R.layout.data_row, parent, false);
//         }
//         else
//         {
//             view = convertView;
//         }
//
//         DataModel model = this.dataModels.get(position);
//
//         EditText titleView = (EditText) view.findViewById(R.id.title);
//         EditText urlView = (EditText) view.findViewById(R.id.url);
//
//         titleView.setText(model.getTitle());
//         urlView.setText(model.getUrl());
//         view.setTag(Integer.valueOf(position));
//
//         // titleView.addTextChangedListener(new DataTextWatcher(position, "TITLE"));
//         // urlView.addTextChangedListener(new DataTextWatcher(position, "URL"));
//
//         titleView.addTextChangedListener(new TextWatcher() {
//             @Override
//             public void afterTextChanged(Editable s)
//             {
//                 DataModel model = dataModels.get(position);
//                 model.setTitle(s.toString());
//             }
//             @Override
//             public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
//             @Override
//             public void onTextChanged(CharSequence s, int start, int before, int count) {}
//         });
//
//         urlView.addTextChangedListener(new TextWatcher() {
//             @Override
//             public void afterTextChanged(Editable s)
//             {
//                 DataModel model = dataModels.get(position);
//                 model.setUrl(s.toString());
//             }
//             @Override
//             public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
//             @Override
//             public void onTextChanged(CharSequence s, int start, int before, int count) {}
//         });
//
//         return view;
//     }
//
//     // public class DataTextWatcher implements TextWatcher
//     // {
//     //     private int pos = -1;
//     //     private String name = "";
//     //
//     //     public DataTextWatcher(int pos, String name)
//     //     {
//     //         this.pos = pos;
//     //         this.name = name;
//     //     }
//     //
//     //     @Override
//     //     public void afterTextChanged(Editable s)
//     //     {
//     //         String str = s.toString();
//     //         if (str == null || pos < 0 || name == null)
//     //             return;
//     //         Log.d("DataTextWatcher", "Changing At: " + String.valueOf(pos));
//     //         DataModel model = dataModels.get(pos);
//     //         if (name == "TITLE")
//     //             model.setTitle(str);
//     //         if (name == "URL")
//     //             model.setUrl(str);
//     //     }
//     //
//     //     @Override
//     //     public void beforeTextChanged(CharSequence s, int start, int count, int after)
//     //     {}
//     //
//     //     @Override
//     //     public void onTextChanged(CharSequence s, int start, int before, int count)
//     //     {}
//     // }
//
//     // public static List<DataModel> intentToList(Intent intent)
//     // {
//     //     ArrayList<DataModel> data = new ArrayList<>();
//     //
//     //     if (intent == null || !Intent.ACTION_SEND.equals(intent.getAction()))
//     //         return data;
//     //
//     //     String fullText = intent.hasExtra(Intent.EXTRA_TEXT) ?
//     //         intent.getStringExtra(Intent.EXTRA_TEXT) : null;
//     //     String fullSubject = intent.hasExtra(Intent.EXTRA_SUBJECT) ?
//     //         intent.getStringExtra(Intent.EXTRA_SUBJECT) : null;
//     //
//     //     Log.i("DataAdapter", "[TEXT]: " + fullText + " [SUBJECT]: " + fullSubject);
//     //
//     //     List<String> titles = new ArrayList<String>(
//     //                             Arrays.asList(fullSubject.split("\n")));
//     //     List<String> urls = new ArrayList<String>(
//     //                             Arrays.asList(fullText.split("\n")));
//     //
//     //     Log.i("DataAdapter", "Texts: " + String.valueOf(titles.size()) + " Subjects: " + String.valueOf(urls.size()));
//     //
//     //     for (int i = 0; i < urls.size(); ++i)
//     //     {
//     //         String u = urls.get(i);
//     //         String t = "";
//     //         if (i < titles.size())
//     //             t = titles.get(i);
//     //         Log.i("DataAdapter", "[TITLE]: " + t + " [URL]: " + u);
//     //         data.add(new DataModel(t, u));
//     //     }
//     //
//     //     return data;
//     // }
// }
