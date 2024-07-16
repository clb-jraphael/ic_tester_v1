import processing.serial.*;
import controlP5.*;

ControlP5 cp5;
Serial port;

Textarea leftTextarea, rightTextarea;
Textfield inputField;
Button sendButton, startButton;

void setup() {
  size(1280, 720);
  cp5 = new ControlP5(this);

  // Left Text Area
  leftTextarea = cp5.addTextarea("resultsTextArea")
                    .setPosition(10, 40)
                    .setSize(615, 670)
                    .setFont(createFont("Monospaced", 16))
                    .setLineHeight(20)
                    .setColor(color(0))
                    .setColorBackground(color(255))
                    .setColorForeground(color(0))
                    .setText("Lorem ipsum dolor sit amet");

  // Right Text Area - Serial Monitor
  rightTextarea = cp5.addTextarea("serialMonitor")
                     .setPosition(655, 40)
                     .setSize(615, 600)
                     .setFont(createFont("Monospaced", 16))
                     .setLineHeight(20)
                     .setColor(color(255))
                     .setColorBackground(color(0))
                     .setColorForeground(color(255))
                     .setText("");

  // Input Field for Serial Monitor
  inputField = cp5.addTextfield("")
                  .setPosition(655, 660)
                  .setSize(500, 30)
                  .setFont(createFont("Monospaced", 16))
                  .setAutoClear(false);

  // Send Button
  sendButton = cp5.addButton("sendButton")
                  .setLabel("Send")
                  .setPosition(1160, 660)
                  .setSize(100, 30)
                  .onClick(new CallbackListener() {
                    public void controlEvent(CallbackEvent event) {
                      if (port != null && port.active()) {
                        port.write(inputField.getText());
                        inputField.clear();
                      } else {
                        rightTextarea.append("\nError: Serial port not connected.");
                        rightTextarea.scroll(1);
                      }
                    }
                  });
             
  // Start Button
  startButton = cp5.addButton("startButton")
                  .setLabel("Start Task")
                  .setPosition(1160, 5)
                  .setSize(100, 30)
                  .onClick(new CallbackListener() {
                    public void controlEvent(CallbackEvent event) {
                      if (port != null && port.active()) {
                        port.write(inputField.getText());
                        inputField.clear();
                      } else {
                        rightTextarea.append("\nError: Serial port not connected.");
                        rightTextarea.scroll(1);
                      }
                    }
                  });
  
  // Menu bar
  addMenu("Communications", 0, new String[]{"COM1 Serial Port", "COM2 Serial Port", "COM3 Serial Port", "COM4 Serial Port", "COM5 Serial Port", "COM6 Serial Port", "COM7 Serial Port", "COM8 Serial Port"});
  addMenu("Automatic", 1, new String[]{"14-PIN", "16-PIN", "8-PIN", "20-PIN"});
  addMenu("Manual", 2, new String[]{"*", "7400", "7402", "7404", "7408", "7432", "7486", "747266"});
  
  // Initialize the port with a default value (optional)
  initializePort("COM3");
}

void addMenu(String name, int index, String[] items) {
  int x = 10 + (index * 200); // Adjust spacing as needed
  DropdownList menu = cp5.addDropdownList(name)
                          .setPosition(x, 10)
                          .setSize(180, 100)
                          .setBarHeight(20)
                          .setItemHeight(20)
                          .close()
                          .bringToFront();  // Ensure it appears above text areas

  // Add items to the dropdown list
  for (int i = 0; i < items.length; i++) {
    menu.addItem(items[i], i);
  }
  
  styleDropdown(menu);
}

void styleDropdown(DropdownList ddl) {
  ddl.setColorBackground(color(200))
     .setColorActive(color(255, 128))
     .setColorForeground(color(100));
}

void controlEvent(ControlEvent theEvent) {
  if (theEvent.isController()) {
    String name = theEvent.getController().getName();
    if (name.equals("Communications") || name.equals("Automatic") || name.equals("Manual")) {
      cp5.get(name).bringToFront();
      
      if (name.equals("Communications")) {
        int index = (int) theEvent.getValue();
        String comPort = "COM" + (index + 1);
        println("Selected Port: " + comPort);
        
        initializePort(comPort);
      }
    }
  }
}

void initializePort(String portName) {
  try {
    if (port != null) {
      port.stop();
    }
    port = new Serial(this, portName, 9600);
    port.bufferUntil('\n');
    rightTextarea.append("\nConnected to " + portName);
    rightTextarea.scroll(1);
  } catch (Exception e) {
    println("Error connecting to port " + portName);
    rightTextarea.append("\nError connecting to port " + portName);
    rightTextarea.scroll(1);
  }
}

void serialEvent(Serial port) {
  String inData = port.readStringUntil('\n'); // Read the data from the serial port
  if (inData != null) {
    inData = inData.trim(); // Remove any leading/trailing whitespace
    rightTextarea.append("\n" + inData); // Append the data to the right text area
    rightTextarea.scroll(1); // Scroll down to the latest text
  }
}

void draw() {
  background(240);
}
