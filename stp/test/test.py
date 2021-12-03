import unittest
from stp import *
import time


class TestBasic(unittest.TestCase):
    def setUp(self):
        """
        Set up test Data
                                   19           62
                    root        |-------SW4(d)------|
        SW3(c)------SW1(a)------SW2(b)     |19      SW6(f)
               4            4   |-------SW5(e)------|
                                   62          62
        """

        self.Switch = []

        self.Switch.append(makeSwitch("00:00:00:00:00:01"))
        self.Switch.append(makeSwitch("00:00:00:00:00:02"))
        self.Switch.append(makeSwitch("00:00:00:00:00:03"))
        self.Switch.append(makeSwitch("00:00:00:00:00:04"))
        self.Switch.append(makeSwitch("00:00:00:00:00:05"))
        self.Switch.append(makeSwitch("00:00:00:00:00:10"))

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
            self.assertEqual(x.getRootId(), 1)
        self.assertEqual(self.Switch[5].getRootPath(), self.Switch[3].getBridgeId())
        self.assertEqual(self.Switch[5].getRootCost(), LINK16 + LINK100 + LINK1000)

    def test_switch(self):
        a = makeSwitch("00:00:00:00:00:05")
        self.assertEqual(a.getBridgeId(), 5)
        self.assertEqual(a.getRootCost(), 0)
        self.assertEqual(a.getRootId(), a.getBridgeId())
        self.assertEqual(a.getRootPath(), a.getBridgeId())

    def test_link(self):
        a = makeSwitch("00:00:00:00:00:02")
        b = makeSwitch("00:00:00:00:00:03")
        link(a, b, LINK100)
        a_neighbors = []
        b_neighbors = []
        at = False
        bt = False
        for tmp in b.getNeighbors():
            x = tmp.lock()
            if a.getBridgeId() == x.getBridgeId():
                at = True
        for tmp in a.getNeighbors():
            x = tmp.lock()
            if b.getBridgeId() == x.getBridgeId():
                bt = True
        self.assertTrue(at)
        self.assertTrue(bt)


if __name__ == "__main__":
    unittest.main()
