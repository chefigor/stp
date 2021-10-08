import unittest
from stp_c import *
import time


class TestBasic(unittest.TestCase):
    def setUp(self):
        # Load test data
        self.Switch = []
        self.Switch.append(Switch(convert_mac("00:00:00:00:00:01")))
        self.Switch.append(Switch(convert_mac("00:00:00:00:00:02")))
        self.Switch.append(Switch(convert_mac("00:00:00:00:00:03")))
        self.Switch.append(Switch(convert_mac("00:00:00:00:00:04")))
        self.Switch.append(Switch(convert_mac("00:00:00:00:00:05")))
        self.Switch.append(Switch(convert_mac("00:00:00:00:00:10")))

        link(self.Switch[0], self.Switch[1], LINK1000)
        link(self.Switch[0], self.Switch[2], LINK1000)
        link(self.Switch[1], self.Switch[3], LINK100)
        link(self.Switch[1], self.Switch[4], LINK16)
        link(self.Switch[4], self.Switch[3], LINK100)
        link(self.Switch[4], self.Switch[5], LINK16)
        link(self.Switch[3], self.Switch[5], LINK16)

    def test_convergence(self):
        for x in self.Switch:
            x.startSwitch()
        time.sleep(10)
        for x in self.Switch:
            x.stopSwitch()
        for x in self.Switch:
            self.assertEqual(x.root_id, 1)
        self.assertEqual(self.Switch[5].root_path,self.Switch[3].bridge_id)
        self.assertEqual(self.Switch[5].root_cost,LINK16+LINK100+LINK1000)


    def test_switch(self):
        a = Switch(5)
        self.assertEqual(a.bridge_id, 5)
        self.assertEqual(a.root_cost, 0)
        self.assertEqual(a.root_id, a.bridge_id)
        self.assertEqual(a.root_path, a.bridge_id)

    def test_link(self):
        a = Switch(2)
        b = Switch(3)
        link(a, b, LINK100)
        a_neighbors = []
        b_neighbors = []
        at = False
        bt = False
        for x in b.neighbors:
            if a.bridge_id == x.sw.bridge_id:
                at = True
        for x in a.neighbors:
            if b.bridge_id == x.sw.bridge_id:
                bt = True
        self.assertTrue(at)
        self.assertTrue(bt)


if __name__ == "__main__":
    unittest.main()
