<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: overlay.proto

namespace Protocol;

use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\RepeatedField;
use Google\Protobuf\Internal\GPBUtil;

/**
 *for ledgers
 *
 * Generated from protobuf message <code>protocol.Ledgers</code>
 */
class Ledgers extends \Google\Protobuf\Internal\Message
{
    /**
     * Generated from protobuf field <code>repeated .protocol.ConsensusValue values = 1;</code>
     */
    private $values;
    /**
     * Generated from protobuf field <code>.protocol.Ledgers.SyncCode sync_code = 2;</code>
     */
    private $sync_code = 0;
    /**
     * Generated from protobuf field <code>int64 max_seq = 3;</code>
     */
    private $max_seq = 0;
    /**
     * Generated from protobuf field <code>bytes proof = 4;</code>
     */
    private $proof = '';

    public function __construct() {
        \GPBMetadata\Overlay::initOnce();
        parent::__construct();
    }

    /**
     * Generated from protobuf field <code>repeated .protocol.ConsensusValue values = 1;</code>
     * @return \Google\Protobuf\Internal\RepeatedField
     */
    public function getValues()
    {
        return $this->values;
    }

    /**
     * Generated from protobuf field <code>repeated .protocol.ConsensusValue values = 1;</code>
     * @param \Protocol\ConsensusValue[]|\Google\Protobuf\Internal\RepeatedField $var
     * @return $this
     */
    public function setValues($var)
    {
        $arr = GPBUtil::checkRepeatedField($var, \Google\Protobuf\Internal\GPBType::MESSAGE, \Protocol\ConsensusValue::class);
        $this->values = $arr;

        return $this;
    }

    /**
     * Generated from protobuf field <code>.protocol.Ledgers.SyncCode sync_code = 2;</code>
     * @return int
     */
    public function getSyncCode()
    {
        return $this->sync_code;
    }

    /**
     * Generated from protobuf field <code>.protocol.Ledgers.SyncCode sync_code = 2;</code>
     * @param int $var
     * @return $this
     */
    public function setSyncCode($var)
    {
        GPBUtil::checkEnum($var, \Protocol\Ledgers_SyncCode::class);
        $this->sync_code = $var;

        return $this;
    }

    /**
     * Generated from protobuf field <code>int64 max_seq = 3;</code>
     * @return int|string
     */
    public function getMaxSeq()
    {
        return $this->max_seq;
    }

    /**
     * Generated from protobuf field <code>int64 max_seq = 3;</code>
     * @param int|string $var
     * @return $this
     */
    public function setMaxSeq($var)
    {
        GPBUtil::checkInt64($var);
        $this->max_seq = $var;

        return $this;
    }

    /**
     * Generated from protobuf field <code>bytes proof = 4;</code>
     * @return string
     */
    public function getProof()
    {
        return $this->proof;
    }

    /**
     * Generated from protobuf field <code>bytes proof = 4;</code>
     * @param string $var
     * @return $this
     */
    public function setProof($var)
    {
        GPBUtil::checkString($var, False);
        $this->proof = $var;

        return $this;
    }

}

