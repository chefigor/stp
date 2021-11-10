import unittest
from stp import *
import time


class TestBasic(unittest.TestCase):
    # def setUp(self):
    #     # Load test data
    #     self.Switch = []
        
    #     self.Switch.append(Switch("00:00:00:00:00:02"))
    #     self.Switch.append(Switch("00:00:00:00:00:03"))
    #     self.Switch.append(Switch("00:00:00:00:00:01"))
    #     self.Switch.append(Switch("00:00:00:00:00:04"))
    #     self.Switch.append(Switch("00:00:00:00:00:05"))
    #     self.Switch.append(Switch("00:00:00:00:00:10"))

    #     link(self.Switch[0], self.Switch[1], LINK1000)
    #     link(self.Switch[0], self.Switch[2], LINK1000)
    #     link(self.Switch[1], self.Switch[3], LINK100)
    #     link(self.Switch[1], self.Switch[4], LINK16)
    #     link(self.Switch[4], self.Switch[3], LINK100)
    #     link(self.Switch[4], self.Switch[5], LINK16)
    #     link(self.Switch[3], self.Switch[5], LINK16)

    # def test_convergence(self):
    #     for x in self.Switch:
    #         x.startSwitch()
    #     time.sleep(10)
    #     for x in self.Switch:
    #         x.stopSwitch()
    #     for x in self.Switch:
    #         self.assertEqual(x.getRootId(), 1)
    #     self.assertEqual(self.Switch[5].getRootPath(),self.Switch[3].getBridgeId())
    #     self.assertEqual(self.Switch[5].getRootCost(),LINK16+LINK100+LINK1000)


    def test_switch(self):
        a = Switch(5)
        self.assertEqual(a.getBridgeId(), 5)
        self.assertEqual(a.getRootCost(), 0)
        self.assertEqual(a.getRootId(), a.getBridgeId())
        self.assertEqual(a.getRootPath(), a.getBridgeId())

    def test_link(self):
        a = Switch(2)
        b = Switch(3)
        link(a, b, LINK100)
        a_neighbors = []
        b_neighbors = []
        at = False
        bt = False
        for x in b.getNeighbors():
            if a.getBridgeId() == x.getBridgeId():
                at = True
        for x in a.getNeighbors():
            if b.getBridgeId() == x.sw.getBridgeId():
                bt = True
        self.assertTrue(at)
        self.assertTrue(bt)


if __name__ == "__main__":
    unittest.main()
