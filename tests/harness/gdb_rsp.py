"""Minimal GDB Remote Serial Protocol client for mGBA's stub (mgba -g)."""
import socket
import time


class GdbClient:
    def __init__(self, host="127.0.0.1", port=2345, timeout=10.0):
        self.sock = socket.create_connection((host, port), timeout=timeout)
        self.sock.settimeout(timeout)
        self.running = False

    def _send(self, payload: str):
        chk = sum(payload.encode()) % 256
        self.sock.sendall(b"$" + payload.encode() + b"#" + f"{chk:02x}".encode())

    def _recv_packet(self) -> str:
        buf = b""
        while True:
            b1 = self.sock.recv(1)
            if not b1:
                raise ConnectionError("gdb stub closed")
            if b1 == b"+" or b1 == b"-":
                continue
            if b1 == b"$":
                buf = b""
                continue
            if b1 == b"#":
                self.sock.recv(2)  # checksum
                self.sock.sendall(b"+")
                return buf.decode()
            buf += b1

    def cmd(self, payload: str) -> str:
        self._send(payload)
        return self._recv_packet()

    def cont(self):
        """Resume execution; returns immediately (no stop reply expected yet)."""
        self._send("c")
        self.running = True

    def interrupt(self):
        """Break into a running target; consumes the stop reply."""
        if not self.running:
            return
        self.sock.sendall(b"\x03")
        self._recv_packet()  # stop reply, e.g. T05...
        self.running = False

    def read_mem(self, addr: int, length: int) -> bytes:
        assert not self.running
        out = b""
        while length > 0:
            n = min(length, 256)
            reply = self.cmd(f"m{addr:x},{n:x}")
            if reply.startswith("E"):
                raise IOError(f"read {addr:#x}: {reply}")
            out += bytes.fromhex(reply)
            addr += n
            length -= n
        return out

    def write_mem(self, addr: int, data: bytes):
        assert not self.running
        reply = self.cmd(f"M{addr:x},{len(data):x}:" + data.hex())
        if reply != "OK":
            raise IOError(f"write {addr:#x}: {reply}")

    def read_u32(self, addr: int) -> int:
        return int.from_bytes(self.read_mem(addr, 4), "little")

    def read_u16(self, addr: int) -> int:
        return int.from_bytes(self.read_mem(addr, 2), "little")

    def read_u8(self, addr: int) -> int:
        return self.read_mem(addr, 1)[0]

    def write_u32(self, addr: int, val: int):
        self.write_mem(addr, val.to_bytes(4, "little"))

    def write_u16(self, addr: int, val: int):
        self.write_mem(addr, val.to_bytes(2, "little"))

    def close(self):
        try:
            self.sock.close()
        except OSError:
            pass
