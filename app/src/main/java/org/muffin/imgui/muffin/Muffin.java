package org.muffin.imgui.muffin;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Handler;
import android.provider.Settings;

public class Muffin extends Activity {

    static {
        System.loadLibrary("imgui");
    }

    public Muffin(Context context) {
        startMuffin(context);
    }

    public static void startMuffin(final Context context) {
        if (!Settings.canDrawOverlays(context)) {
            AlertDialog.Builder builder = new AlertDialog.Builder(context);
            builder.setMessage("'Display over other apps' permission is mandatory to run this application.")
                    .setTitle("Permission required")
                    .setCancelable(false)
                    .setPositiveButton("Open settings", (dialog, which) -> {
                        context.startActivity(new Intent("android.settings.action.MANAGE_OVERLAY_PERMISSION", Uri.parse("package:" + context.getPackageName())));
                        dialog.dismiss();
                        System.exit(1);
                    })
                    .create().show();
        } else {
            new Handler(context.getMainLooper()).postDelayed(() -> context.startService(new Intent(context, MuffinService.class)), 5000);
        }
    }

}
