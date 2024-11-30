package com.estebandev.core.utils.notify;

import javax.swing.JOptionPane;
import java.io.IOException;
import java.util.Locale;

public class SendNotifyImpl implements SendNotifyI {

    @Override
    public void send(String message) {
        String os = System.getProperty("os.name").toLowerCase(Locale.ROOT);

        try {
            if (os.contains("linux")) {
                sendLinuxNotification(message);
            } else if (os.contains("win")) {
                sendWindowsNotification(message);
            } else if (os.contains("mac")) {
                sendMacNotification(message);
            } else {
                fallbackNotification(message);
            }
        } catch (IOException e) {
            System.err.println("Error sending native notification: " + e.getMessage());
            fallbackNotification(message);
        }
    }

    private void sendLinuxNotification(String message) throws IOException {
        new ProcessBuilder("notify-send", "Notification", message).start();
    }

    private void sendWindowsNotification(String message) throws IOException {
        new ProcessBuilder(
            "powershell.exe",
            "-Command",
            "[Windows.UI.Notifications.ToastNotificationManager, Windows.UI.Notifications, ContentType = WindowsRuntime] | Out-Null; " +
            "$template = [Windows.UI.Notifications.ToastNotificationManager]::GetTemplateContent([Windows.UI.Notifications.ToastTemplateType]::ToastText02); " +
            "$text = $template.GetElementsByTagName('text'); " +
            "$text[0].AppendChild($template.CreateTextNode('Notification')) | Out-Null; " +
            "$text[1].AppendChild($template.CreateTextNode('" + message + "')) | Out-Null; " +
            "$toast = [Windows.UI.Notifications.ToastNotification]::new($template); " +
            "[Windows.UI.Notifications.ToastNotificationManager]::CreateToastNotifier('AppName').Show($toast)"
        ).start();
    }

    private void sendMacNotification(String message) throws IOException {
        new ProcessBuilder("osascript", "-e",
            "display notification \"" + message + "\" with title \"Notification\"").start();
    }

    private void fallbackNotification(String message) {
        JOptionPane.showMessageDialog(null, message, "Notification", JOptionPane.INFORMATION_MESSAGE);
    }
}

