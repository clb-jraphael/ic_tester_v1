import processing.serial.*;
import controlP5.*;

ControlP5 cp5;
Serial port;

Textarea leftTextarea, rightTextarea;
Textfield inputField;
Button sendButton, startButton;

String[] auto = {"14-PIN", "16-PIN", "8-PIN", "20-PIN"};
String[] manual = {"7400", "7402", "7404", "7408", "7432", "7486", "747266"};

RadioButton autoManualSelector;

DropdownList commDropdown, autoDropdown, manualDropdown;

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
                      executeTask();
                    }
                  });

  // Menu bar
  commDropdown = addMenu("Communications", 0, new String[0]); // Empty list initially
  autoDropdown = addMenu("Automatic", 1, auto);
  manualDropdown = addMenu("Manual", 2, manual);

  // Populate commDropdown with available serial ports
  updateCommDropdown();

  // Initialize the port with a default value (optional)
  if (commDropdown.getItems().size() > 0) {
    initializePort(commDropdown.getItem(0).get("name").toString());
  }

  // Radio buttons for mode selection
  autoManualSelector = cp5.addRadioButton("autoManualSelector")
                           .setPosition(655, 10)
                           .setSize(20, 20)
                           .setItemsPerRow(2)
                           .setSpacingColumn(100) // Adjust spacing between buttons
                           .addItem("Automatic Mode", 0)
                           .addItem("Manual Mode", 1)
                           .activate(0); // Default selection
}

DropdownList addMenu(String name, int index, String[] items) {
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
  return menu;
}

void styleDropdown(DropdownList ddl) {
  ddl.setColorBackground(color(200))
     .setColorActive(color(255, 128))
     .setColorForeground(color(100));
}

void updateCommDropdown() {
  String[] availablePorts = Serial.list();
  commDropdown.clear(); // Clear existing items
  for (int i = 0; i < availablePorts.length; i++) {
    commDropdown.addItem(availablePorts[i], i);
  }
  commDropdown.bringToFront(); // Ensure it appears above other elements
}

void controlEvent(ControlEvent theEvent) {
  if (theEvent.isController()) {
    String name = theEvent.getController().getName();
    if (name.equals("Communications")) {
      cp5.get(name).bringToFront();
      
      int index = (int) theEvent.getValue();
      String comPort = commDropdown.getItem(index).get("name").toString();
      println("Selected Port: " + comPort);
      
      initializePort(comPort);
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

void executeTask() {
  if (port != null && port.active()) {
    int mode = (int) autoManualSelector.getValue();
    println("Mode: " + mode);

    if (mode == 0) { // Automatic Mode
      String selectedAuto = autoDropdown.getItem((int)autoDropdown.getValue()).get("name").toString();
      println("Selected Automatic: " + selectedAuto);
      switch (selectedAuto) {
        case "14-PIN":
          port.write("1");
          rightTextarea.append("\nSent: 1 for " + selectedAuto);
          break;
        case "16-PIN":
          port.write("2");
          rightTextarea.append("\nSent: 2 for " + selectedAuto);
          break;
        case "8-PIN":
          port.write("3");
          rightTextarea.append("\nSent: 3 for " + selectedAuto);
          break;
        case "20-PIN":
          port.write("4");
          rightTextarea.append("\nSent: 4 for " + selectedAuto);
          break;
        default:
          rightTextarea.append("\nNo action defined for selected Automatic option.");
          rightTextarea.scroll(1);
          break;
      }
    } else if (mode == 1) { // Manual Mode
      String selectedManual = manualDropdown.getItem((int)manualDropdown.getValue()).get("name").toString();
      println("Selected Manual: " + selectedManual);
      switch (selectedManual) {
        case "7400":
          port.write("a");
          rightTextarea.append("\nSent: a for " + selectedManual);
          break;
        case "7402":
          port.write("b");
          rightTextarea.append("\nSent: b for " + selectedManual);
          break;
        case "7404":
          port.write("c");
          rightTextarea.append("\nSent: c for " + selectedManual);
          break;
        case "7408":
          port.write("d");
          rightTextarea.append("\nSent: d for " + selectedManual);
          break;
        case "7432":
          port.write("e");
          rightTextarea.append("\nSent: e for " + selectedManual);
          break;
        case "7486":
          port.write("f");
          rightTextarea.append("\nSent: f for " + selectedManual);
          break;
        case "747266":
          port.write("g");
          rightTextarea.append("\nSent: g for " + selectedManual);
          break;
        default:
          rightTextarea.append("\nNo action defined for selected Manual option.");
          rightTextarea.scroll(1);
          break;
      }
    } else {
      rightTextarea.append("\nError: No mode selected.");
      rightTextarea.scroll(1);
    }
  } else {
    rightTextarea.append("\nError: Serial port not connected.");
    rightTextarea.scroll(1);
  }
}

void draw() {
  background(14, 156, 75);
}
