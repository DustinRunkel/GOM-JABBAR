import sys
import usb.core
import usb.util
import usb.backend.libusb1
import usb.backend.openusb
import usb.backend.libusb0
from PyQt6.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel
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

        # Polls the USB Port every 3 seconds to determine if new USB 
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.check_usb_status)
        self.timer.start(3000)

        self.check_usb_status()

    def get_available_backend(self) -> usb.backend | None:
        """Tries all known USB backends and returns the first one that works."""
        for backend in [usb.backend.libusb1, usb.backend.openusb, usb.backend.libusb0]:
            try:
                if backend.get_backend():
                    return backend.get_backend()
            except Exception:
                continue 
        return None  

    def check_usb_status(self) -> None:
        # Find an available USB backend
        backend = self.get_available_backend()

        if backend is None:
            self.status_label.setText("❌ No available USB backend!")
            self.status_label.setStyleSheet("color: orange; font-size: 16px;")
            return

        try:
            # Try to find the device with the working backend
            device = usb.core.find(idVendor=self.vendor_id, idProduct=self.product_id, backend=backend)

            if device:
                self.status_label.setText("✅ Raspberry Pi Connected")
                self.status_label.setStyleSheet("color: green; font-size: 16px;")
            else:
                self.status_label.setText("❌ Raspberry Pi Disconnected")
                self.status_label.setStyleSheet("color: red; font-size: 16px;")

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
