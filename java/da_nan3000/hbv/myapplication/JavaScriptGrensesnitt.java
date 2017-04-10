package da_nan3000.hbv.myapplication;

import android.database.Cursor;
import android.widget.Toast;
import android.support.v4.content.ContextCompat;
import android.Manifest;
import android.content.pm.PackageManager;
import android.provider.ContactsContract.Contacts;
import android.provider.ContactsContract.CommonDataKinds.Phone;



public class JavaScriptGrensesnitt extends android.app.Activity {

    android.content.Context kontekst;

    public JavaScriptGrensesnitt(android.content.Context kontekst) {
        super();
        this.kontekst = kontekst;
    }

    @android.webkit.JavascriptInterface
    public String getContacts(String id) {
        String xml = "<phonebook>";


        if (ContextCompat.checkSelfPermission(kontekst, Manifest.permission.READ_CONTACTS) != PackageManager.PERMISSION_GRANTED) {
            visToast("Required persmissions missing...");
            return xml + "</phonebook>";
        }

        Cursor k = kontekst.getContentResolver().query(Contacts.CONTENT_URI, null, null, null, Contacts._ID);
        if (k.getCount() > 0) {
            while (k.moveToNext()) {

                if (k.getString(k.getColumnIndex(Contacts._ID)).equals(id) || id.equals("all")) {
                    if (!k.getString(k.getColumnIndex(Contacts.HAS_PHONE_NUMBER)).equals("1")) {
                        continue;
                    }

                    xml += "<person>";

                    xml += "<id>" + k.getString(k.getColumnIndex(Contacts._ID)) + "</id>";
                    xml += "<tlf>" + getPhoneNumbers(k.getString(k.getColumnIndex(Contacts._ID))) + "</tlf>";
                    xml += "<name>" + k.getString(k.getColumnIndex(Contacts.DISPLAY_NAME)) + "</name>";


                    xml += "</person>";
                }
            }
        }
        k.close();

        xml += "</phonebook>";
        return xml;
    }

    @android.webkit.JavascriptInterface
    public String getPhoneNumbers(String id) {
        String phonenumbers = "";

        Cursor n = kontekst.getContentResolver().query(Phone.CONTENT_URI, null, Phone.CONTACT_ID + " = " + id, null, null);
        while (n.moveToNext()) {
            phonenumbers += n.getString(n.getColumnIndex(Phone.NUMBER)) + "\r\n";
        }
        n.close();

        return phonenumbers;
    }

    @android.webkit.JavascriptInterface
    public void visToast(String text) {
        Toast.makeText(kontekst, text, android.widget.Toast.LENGTH_SHORT).show();
    }
}