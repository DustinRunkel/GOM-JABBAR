import sys
import json
import usb.core
import usb.util
import usb.backend.libusb1
import usb.backend.openusb
import usb.backend.libusb0
import serial  # For Serial Communication
import serial.tools.list_ports # To find serial port
from datetime import datetime
from PyQt6.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QTextEdit
from PyQt6.QtCore import QTimer, Qt

class USBMonitorApp(QWidget):
    def __init__(self, vendor_id, product_id):
        super().__init__()
            
        self.vendor_id = vendor_id
        self.product_id = product_id
        self.setWindowTitle("USB Connection Monitor (WSL)")
        self.setGeometry(100, 100, 300, 150)

        self.layout = QVBoxLayout()
        self.status_label = QLabel("Checking USB connection...", self)
        self.status_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.layout.addWidget(self.status_label)
        self.setLayout(self.layout)

        # Serial Status Label
        self.serial_label = QLabel("No Serial connection...", self)
        self.serial_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.layout.addWidget(self.serial_label)

        # JSON Output Display (new)
        self.json_dis = QTextEdit(self) 
        self.json_dis.setReadOnly(True)
        self.layout.addWidget(self.json_dis)

        self.setLayout(self.layout)

        # Polls the USB Port every 3 seconds to determine if new USB 
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.check_usb_status)
        self.timer.start(3000)

        #Polls the serial connection every second
        self.serial_port = None
        self.serial_timer = QTimer(self)
        self.serial_timer.timeout.connect(self.read_serial_data)
        self.serial_timer.start(1000)

        self.check_usb_status()

    def get_available_backend(self):
        """Tries all known USB backends and returns the first one that works."""
        for backend in [usb.backend.libusb1, usb.backend.openusb, usb.backend.libusb0]:
            try:
                if backend.get_backend():
                    return backend.get_backend()
            except Exception:
                continue 
        return None  

    def json_message(self, message, connection, usb="No USB Connection",serial_data=None):
        """Converts the message to JSON and displays it in the QTextEdit widget."""
        json_info = {
            "datetime": datetime.now().isoformat(),
            "source": "GUI",
            "device": "Raspberry Pi",
            "message": message,
            "level": "INFO" if connection is True else "ERROR",
            "connection": connection,
            "usb": usb if usb else "No USB Connection",
            serial_data: serial_data if serial_data else "No Serial Data" #Serial Data
        }
        json_message = json.dumps(json_info, indent=4)
        self.json_dis.setText(json_message)
        return json_message
    
    # This is used to find the serial port
    def find_serial_port(self):
        ports = serial.tools.list_ports.comports()
        for port in ports:
            if (port.vid == self.vendor_id and port.pid == self.product_id):
                return port.device
        return None
    
    # This is to connect to the serial port
    def connect_serial(self):
        if self.serial_port is None or not self.serial_port.is_open:
            port = self.find_serial_port()
            if port:
                try:
                    self.serial_port = serial.Serial(port, 115200, timeout=1)
                    self.serial_label.setText("Serial Connection Established")
                    self.serial_label.setStyleSheet("color: green; font-size: 16px;")
                except serial.SerialException as e:
                    self.serial_label.setText(f"Serial Error: {str(e)}")
                    self.serial_label.setStyleSheet("color: orange; font-size: 16px;")
            else:
                self.serial_label.setText("No Serial Connection")
                self.serial_label.setStyleSheet("color: red; font-size: 16px;")
                self.json_message("No Serial Connection", connection=False)

    # This is to read the serial data
def read_serial_data(self):
    if self.serial_port.is_open:
        try:
            if self.serial_port.in_waiting > 0:
                serial_data = self.serial_port.readline().decode('utf-8').strip()
                current_text = self.json_dis.toPlainText()
               
                if not current_text:
                    json_data = {}
                else:
                    json_data = json.loads(current_text)
                json_data['serial_data'] = serial_data
                self.json_dis.setText(json.dumps(json_data, indent=4))
        except (serial.SerialException, json.JSONDecodeError) as e:
            self.serial_label.setText(f"Serial error: {str(e)}")
            self.serial_label.setStyleSheet("color: orange; font-size: 16px;")
            self.serial_port.close()
            self.serial_port = None

    def check_usb_status(self) -> None:
        # Find an available USB backend
        backend = self.get_available_backend()

        if backend is None:
            self.status_label.setText("❌ No available USB backend!")
            self.status_label.setStyleSheet("color: orange; font-size: 16px;")
            self.json_message("No available USB backend!", connection=False)
            return

        try:
            # Try to find the device with the working backend
            device = usb.core.find(idVendor=self.vendor_id, idProduct=self.product_id, backend=backend)

            if device:
                self.status_label.setText("✅ Raspberry Pi Connected")
                self.status_label.setStyleSheet("color: green; font-size: 16px;")
                self.json_message("Raspberry Pi Connected", connection=True, usb="Connected")
                self.connect_serial()
            else:
                self.status_label.setText("❌ Raspberry Pi Disconnected")
                self.status_label.setStyleSheet("color: red; font-size: 16px;")
                self.json_message("Raspberry Pi Disconnected", connection=False)
                if self.serial_port:
                    self.serial_port.close()
                    self.serial_port = None
                    self.serial_label.setText("No Serial Connection")

        except usb.core.USBError as e:
            self.status_label.setText(f"❌ USB Error: {str(e)}")
            self.status_label.setStyleSheet("color: orange; font-size: 16px;")

        


if __name__ == "__main__":
    app = QApplication(sys.argv)

    # Vendor ID For Raspberry PI
    VENDOR_ID = 0x2E8A  
    # Product ID For RP-2040
    PRODUCT_ID = 0x0003  

    window = USBMonitorApp(VENDOR_ID, PRODUCT_ID)
    window.show()

    sys.exit(app.exec())


