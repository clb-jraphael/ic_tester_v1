import processing.serial.*; //<>// //<>//
import controlP5.*;

import java.awt.datatransfer.StringSelection;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;

ControlP5 cp5;
Serial port;

Textarea leftTextarea, rightTextarea;
Textfield inputField;
Button sendButton, startButton, copyResultButton, copySerialButton;

boolean isReceivingAutoResults = false; // Flag to indicate if automatic results are being received
String autoResultBuffer = ""; // Buffer to hold the multi-line automatic test results
String[] auto = {"14-PIN", "16-PIN", "8-PIN", "20-PIN"};
String[] manual = {"7400", "7402", "7404", "7408", "7432", "7486", "747266",
                   "7427", "74151", "7421", "74192", "7474", "74190", "74193",
                   "74195", "7410", "7411", "74125", "74138", "7447", "74173",
                   "4070", "4071", "4017", "4511", "4081", "4077", "4068",
                   "4069", "4066", "4094", "74112", "741", "072", "74373"};
PImage logo;

RadioButton autoManualSelector;

DropdownList commDropdown, autoDropdown, manualDropdown;

boolean newResultReady = false; // Flag to check if a new summary result is ready
String resultSummary = ""; // Variable to hold the summary result

/**
* Sets up the GUI components, loads the logo, sets up text areas, input fields, buttons, and dropdown lists.
*/
void setup() {
  size(1280, 720);
  cp5 = new ControlP5(this);
  
  logo = loadImage("layad_logo.png");
  surface.setIcon(logo); // Set the icon for the application window
  surface.setTitle("LC IC Tester");

  // Left Text Area
  leftTextarea = cp5.addTextarea("resultsTextArea")
                    .setPosition(10, 40)
                    .setSize(615, 600)
                    .setFont(createFont("Monospaced", 16))
                    .setLineHeight(20)
                    .setColor(color(0))
                    .setColorBackground(color(255))
                    .setColorForeground(color(0))
                    .setText("");

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
                  
  // Copy Result Button
  copyResultButton = cp5.addButton("copyResultButton")
                      .setLabel("Copy Results to Clipboard")
                      .setPosition(10, 660)
                      .setSize(150, 30)
                      .onClick(new CallbackListener() {
                        public void controlEvent(CallbackEvent event) {
                          String textToCopy = leftTextarea.getText();
                          StringSelection stringSelection = new StringSelection(textToCopy);
                          Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
                          clipboard.setContents(stringSelection, null);
                          println("Results copied to clipboard.");
                        }
                      });

  // Copy Serial Button
  copySerialButton = cp5.addButton("copySerialButton")
                      .setLabel("Copy Serial Results to Clipboard")
                      .setPosition(170, 660)
                      .setSize(150, 30)
                      .onClick(new CallbackListener() {
                        public void controlEvent(CallbackEvent event) {
                          String textToCopy = rightTextarea.getText();
                          StringSelection stringSelection = new StringSelection(textToCopy);
                          Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
                          clipboard.setContents(stringSelection, null);
                          println("Results copied to clipboard.");
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

/**
* Adds a dropdown list menu with the given name, index, and items.
* 
* @param name  The name of the dropdown list
* @param index The index of the dropdown list
* @param items The items to be added to the dropdown list
* @return The created DropdownList object
*/
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

/**
* Styles a dropdown list with specified colors.
* 
* @param ddl The DropdownList to be styled
*/
void styleDropdown(DropdownList ddl) {
  ddl.setColorBackground(color(200))
     .setColorLabel(color(0))
     .setColorActive(color(255, 128))
     .setColorForeground(color(100));
}

/**
* Updates the communication dropdown list with available serial ports.
*/
void updateCommDropdown() {
  String[] availablePorts = Serial.list();
  commDropdown.clear(); // Clear existing items
  for (int i = 0; i < availablePorts.length; i++) {
    commDropdown.addItem(availablePorts[i], i);
  }
  commDropdown.bringToFront(); // Ensure it appears above other elements
}

/**
* Handles control events for the dropdown lists and other controls.
* 
* @param theEvent The control event to be handled
*/
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

/**
* Initializes the serial port with the given port name.
* 
* @param portName The name of the serial port to initialize
*/
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

/**
* Handles serial events and processes incoming data from the serial port.
* 
* @param port The serial port from which data is received
*/
void serialEvent(Serial port) {
  String inData = port.readStringUntil('\n'); // Read the data from the serial port
  if (inData != null) {
    inData = inData.trim(); // Remove any leading/trailing whitespace
    rightTextarea.append("\n" + inData); // Append the data to the right text area
    rightTextarea.scroll(1); // Scroll down to the latest text

    // Check for summary results for manual and update the left textarea
    if (inData.contains("passed all tests") || inData.contains("failed")) {
      newResultReady = true;
      resultSummary = inData;
    }

    // Check for summary results for automatic and start buffering the results
    if (inData.contains("Passed Models:") || inData.contains("No models passed")) {
      isReceivingAutoResults = true;
      autoResultBuffer = inData + "\n"; // Start buffering the results
    } else if (isReceivingAutoResults) {
      if (inData.isEmpty()) {
        isReceivingAutoResults = false; // Stop buffering when an empty line is encountered
        newResultReady = true;
        resultSummary = autoResultBuffer; // Set the result summary to the buffered results
      } else {
        autoResultBuffer += inData + "\n"; // Continue buffering the results
      }
    }
  }
}

/**
* Executes the task based on the selected mode (automatic or manual) and sends corresponding
* signal to the IC tester.
*/
void executeTask() {
  if (port != null && port.active()) {
    int mode = (int) autoManualSelector.getValue();
    println("Mode: " + mode);

    if (mode == 0) { // Automatic Mode
      String selectedAuto = autoDropdown.getItem((int)autoDropdown.getValue()).get("name").toString();
      println("Selected Automatic: " + selectedAuto);
      switch (selectedAuto) {
        case "14-PIN":
          port.write("0");
          rightTextarea.append("\nSent: 0 for " + selectedAuto);
          break;
        case "16-PIN":
          port.write("1");
          rightTextarea.append("\nSent: 1 for " + selectedAuto);
          break;
        case "8-PIN":
          port.write("2");
          rightTextarea.append("\nSent: 2 for " + selectedAuto);
          break;
        case "20-PIN":
          port.write("3");
          rightTextarea.append("\nSent: 3 for " + selectedAuto);
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
          port.write("4");
          rightTextarea.append("\nSent: 4 for " + selectedManual);
          break;
        case "7402":
          port.write("5");
          rightTextarea.append("\nSent: 5 for " + selectedManual);
          break;
        case "7404":
          port.write("6");
          rightTextarea.append("\nSent: 6 for " + selectedManual);
          break;
        case "7408":
          port.write("7");
          rightTextarea.append("\nSent: 7 for " + selectedManual);
          break;
        case "7432":
          port.write("8");
          rightTextarea.append("\nSent: 8 for " + selectedManual);
          break;
        case "7486":
          port.write("9");
          rightTextarea.append("\nSent: 9 for " + selectedManual);
          break;
        case "747266":
          port.write("a");
          rightTextarea.append("\nSent: a for " + selectedManual);
          break;
        case "7427":
          port.write("b");
          rightTextarea.append("\nSent: b for " + selectedManual);
          break;
        case "74151":
          port.write("c");
          rightTextarea.append("\nSent: c for " + selectedManual);
          break;
        case "7421":
          port.write("d");
          rightTextarea.append("\nSent: d for " + selectedManual);
          break;
        case "74192":
          port.write("e");
          rightTextarea.append("\nSent: e for " + selectedManual);
          break;
        case "7474":
          port.write("f");
          rightTextarea.append("\nSent: f for " + selectedManual);
          break;
        case "74190":
          port.write("g");
          rightTextarea.append("\nSent: g for " + selectedManual);
          break;
        case "74193":
          port.write("h");
          rightTextarea.append("\nSent: h for " + selectedManual);
          break;
        case "74195":
          port.write("i");
          rightTextarea.append("\nSent: i for " + selectedManual);
          break;
        case "7410":
          port.write("j");
          rightTextarea.append("\nSent: j for " + selectedManual);
          break;
        case "7411":
          port.write("k");
          rightTextarea.append("\nSent: k for " + selectedManual);
          break;
        case "74125":
          port.write("l");
          rightTextarea.append("\nSent: l for " + selectedManual);
          break;
        case "74138":
          port.write("m");
          rightTextarea.append("\nSent: m for " + selectedManual);
          break;
        case "7447":
          port.write("n");
          rightTextarea.append("\nSent: n for " + selectedManual);
          break;
        case "74173":
          port.write("o");
          rightTextarea.append("\nSent: o for " + selectedManual);
          break;
        case "4070":
          port.write("p");
          rightTextarea.append("\nSent: p for " + selectedManual);
          break;
        case "4071":
          port.write("q");
          rightTextarea.append("\nSent: q for " + selectedManual);
          break;
        case "4017":
          port.write("r");
          rightTextarea.append("\nSent: r for " + selectedManual);
          break;
        case "4511":
          port.write("s");
          rightTextarea.append("\nSent: s for " + selectedManual);
          break;
        case "4081":
          port.write("t");
          rightTextarea.append("\nSent: t for " + selectedManual);
          break;
        case "4077":
          port.write("u");
          rightTextarea.append("\nSent: u for " + selectedManual);
          break;
        case "4068":
          port.write("v");
          rightTextarea.append("\nSent: v for " + selectedManual);
          break;
        case "4069":
          port.write("w");
          rightTextarea.append("\nSent: w for " + selectedManual);
          break;
        case "4066":
          port.write("x");
          rightTextarea.append("\nSent: x for " + selectedManual);
          break;
        case "4094":
          port.write("y");
          rightTextarea.append("\nSent: y for " + selectedManual);
          break;
        case "74112":
          port.write("z");
          rightTextarea.append("\nSent: z for " + selectedManual);
          break;
        case "741":
          port.write("A");
          rightTextarea.append("\nSent: A for " + selectedManual);
          break;
        case "072":
          port.write("B");
          rightTextarea.append("\nSent: B for " + selectedManual);
          break;
        case "74373":
          port.write("C");
          rightTextarea.append("\nSent: C for " + selectedManual);
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

/**
* Continuously executes the lines of code contained inside its block until the program is stopped.
*/
void draw() { 
  background(14, 156, 75);

  // Check if a new result summary is ready
  if (newResultReady) {
    leftTextarea.append("\n" + resultSummary);
    leftTextarea.scroll(1);
    newResultReady = false;
  }
}
