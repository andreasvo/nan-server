package da_nan3000.hbv.myapplication;

import java.util.ArrayList;
import android.app.Activity;
import android.content.ContentProviderOperation;
import android.content.ContentUris;
import android.content.ContentValues;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.provider.ContactsContract.CommonDataKinds.StructuredName;
import android.provider.ContactsContract.RawContacts;
import android.provider.ContactsContract.RawContacts.Data;
import android.widget.Toast;

public class JavaScriptGrensesnitt extends android.app.Activity {

    android.content.Context kontekst;

    public JavaScriptGrensesnitt(android.content.Context kontekst) {
        super();
        this.kontekst = kontekst;
    }

    @android.webkit.JavascriptInterface
    protected String getContacts(String searchID) {

        android.webkit.WebView wv = new android.webkit.WebView(this);
        setContentView(wv);

        String xml = "<phonebook>";
        String id = "";
        String name = "";
        String tlf = "";

        Cursor k = getContentResolver().query(ContactsContract.Contacts.CONTENT_URI, null, null, null, null);
        if(k.getCount() > 0) {

            while (k.moveToNext()) {

                id = k.getString(k.getColumnIndex(ContactsContract.Contacts._ID));

                name = k.getString(k.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));

                tlf = k.getString(k.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));

                xml += "<person><id>" + id + "</id><tlf>" + tlf + "</tlf><name>" + name + "</name></person>";
            }
        }

        k.close();
        wv.loadData(xml,"application/xml","utf-8");
        xml += "</phonebook>";

        return xml;

    }

    @android.webkit.JavascriptInterface   // ved target >= 17 må dette være med
    public void visToast(String tekst) {
        Toast.makeText(kontekst, tekst, android.widget.Toast.LENGTH_SHORT).show();
    }
}