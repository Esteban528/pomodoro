package com.estebandev.core;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CompletableFuture;

import com.estebandev.core.utils.display.Display;
import com.estebandev.core.utils.display.DisplayImpl;
import com.estebandev.core.utils.notify.SendNotifyI;
import com.estebandev.core.utils.notify.SendNotifyImpl;

public class PomodoroCore {
    private final PropertyChangeSupport propertyChangeSupport;
    private final Display display;
    private final SendNotifyI notifyHandler;
    private final Map<String, Runnable> options = new LinkedHashMap<>();
    private CompletableFuture<Void> mainTask;
    private CompletableFuture<Void> subTask;
    private List<String> keyOptionList;
    private String value = "null";

    public PomodoroCore() {
        this.propertyChangeSupport = new PropertyChangeSupport(this);
        this.display = new DisplayImpl();
        this.notifyHandler = new SendNotifyImpl();
    }

    public void run() {
        loadOptions();

        this.mainTask = CompletableFuture.runAsync(() -> {
            while (true) {
                display.clear();
                display.addLog("Welcome to pomodoro timer");
                display.addLog("");
                display.addLog("Select an option");
                display.addLog("");

                try {
					Thread.sleep(400);
				} catch (InterruptedException e) {}

                keyOptionList.forEach((key) -> {
                    if (value.equals("running") && key.equalsIgnoreCase("start")) {
                    } else
                        display.addLog(String.format("%d. %s", keyOptionList.indexOf(key) + 1, key));
                });

                display.show();

                int scanOption = display.scan() - 1;
                String value = keyOptionList
                        .get((scanOption < 0 || scanOption < keyOptionList.size()) ? scanOption : 1);
                options.get(value).run();
                if (value.equalsIgnoreCase("exit"))
                    break;
            }
        });

        this.mainTask.join();
        propertyChangeSupport.addPropertyChangeListener("subtask",
                (eventListener) -> onSubtaskPropertyChange(eventListener));
    }

    private void loadOptions() {
        options.put("Start", () -> {
            if (subTask != null && !subTask.isDone())
                return;

            this.subTask = CompletableFuture.runAsync(() -> {
                int loops = 0;
                while (true) {
                    try {
                        setValue("running");
                        notifyHandler.send("> (+) Start focused time");
                        Thread.sleep(toMinute(25));
                        notifyHandler.send("(-) Focused time finished");
                        if (loops++ >= 4) {
                            loops = 0;
                            notifyHandler.send("(=) Rest 15 minutes");
                            Thread.sleep(toMinute(15));
                            continue;
                        }
                        notifyHandler.send("(=) Rest 5 minutes");
                        Thread.sleep(toMinute(5));
                    } catch (InterruptedException e) {
                        notifyHandler.send("Error ocurred. See your terminal");
                        e.printStackTrace();
                        setValue("stopped");
                    }
                }
            });

            setValue("ready");
        });

        options.put("Stop", () -> {
            if (subTask == null)
                return;
            this.subTask.cancel(true);
            notifyHandler.send("(!) Stop pomodoro timer");
            setValue("stopped");
        });

        options.put("Exit", () -> {
            options.get("Stop").run();
        });

        this.keyOptionList = new ArrayList<>(options.keySet().size());
        options.keySet().forEach(key -> {
            keyOptionList.add(key);
        });
    }

    private int toMinute(int minutes) {
        return (1000*60) * minutes;
    }

    private void onSubtaskPropertyChange(PropertyChangeEvent propertyChangeEvent) {
        if (propertyChangeEvent.getNewValue().equals("ready"))
            this.subTask.join();
    }

    private void setValue(String newValue) {
        propertyChangeSupport.firePropertyChange("subtask", this.value, newValue);
        this.value = newValue;
    }
}
