// Copyright (c) 2019-2020 Xenios SEZC
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/test/unit_test.hpp>
#include <chainparams.h>
#include <consensus/validation.h>
#include <test/util/setup_common.h>
#include <validation.h>
#include <vbk/pop_service.hpp>
#include <vbk/test/util/consts.hpp>
#include <vbk/test/util/e2e_fixture.hpp>

#include <string>

inline std::vector<uint8_t> operator""_v(const char* s, size_t size)
{
    return std::vector<uint8_t>{s, s + size};
}

BOOST_AUTO_TEST_SUITE(block_validation_tests)

BOOST_AUTO_TEST_CASE(BadBlock) {
    std::string badblock = "00000020e2566a3dd9d7615e15931d60d06f24b6ba53d71031ed8efdd1a970240100000068140e104d8e931e922683dce3a352877c84cb8b59aab74fa36af89bfdcb7f627d974260e83f2a1c41ca041a0101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff1f021607047d9742600850000000000000000d2f6e6f64655374726174756d2f00000000040000000000000000266a24aa21a9ede2f61c3f71d1defd3fa999dfa36953755c690689799962b48bebd836974e8cf900f2f3af000000001976a914fea78b7efa774694aa66246e7d1ee9d2b82f199388ac40a5ae02000000001976a914ecf809f1ec0ba4faa909d5175e405902a21282be88acc0c62d00000000001976a91422851477d63a085dbc2398c8430af1c09e7343f688ac00000000fd640300000001010d41001483780002faf5a5a587fa7e245ee9464f16ebe5be16b244299b137604b972d23f3566a53ec05c3a137fe513eef9ccd920c8ae604295d5040c6b2c0007fba62141001483790002f9fadbe4e97623bddff3b1d316ebe5be16b244299b137604b972d23f356625dcb9aa2c17bcd30c75f26c3323f68e6042964b040bf6f80007f1bd97410014837a000274234ab0b9ea0bceca9e59bb16ebe5be16b244299b137604b972d23f35661501298eab6466b22e98dd8b4ba3160f6042965e040b4a9b00062f47e0410014837b0002b89017602d099217269898a916ebe5be16b244299b137604b972d23f356691725c54095e949d18e29895e8fb73f160429681040b61380007b2d773410014837c00027175b265dc3ee4972430d40216ebe5be16b244299b137604b972d23f35668dd3fbd113cc24113ba98d8cd0f9a83b60429694040b55ad00001c45da410014837d00028e490eed10805e7bedca493a16ebe5be16b244299b137604b972d23f356657f5e859055cbe674ea23afbdd47971b604296aa040b6b1200026af5d7410014837e0002635c6f7b162e9c4a0053819ced10805e7bedca493a16ebe5be16b244299b08dfc228eddccad10f76ffbe65a31061604296cb040b7af300060afa0f410014837f0002566af07065ce3983ff989b55ed10805e7bedca493a16ebe5be16b244299b7f0ddbb323b74f2e5e6ed8f516ebcffc604296da040b75ac000197fe6841001483800002f6c3677fe7a9e468a442b99bed10805e7bedca493a16ebe5be16b244299becc484f9cbc2b66f2f263d17d79fb32e604296de040b92f9000324e70241001483810002a1e4e8a8c36299f0be264f66ed10805e7bedca493a16ebe5be16b244299b3267f30d2a0e9943ad2ab5f343f8632d604296e9040bc68900031a331c4100148382000235b93d00923f4f283ae60c62ed10805e7bedca493a16ebe5be16b244299b111ccd3fcb698728f4fa643796be9cab604296fb040bed6800062bdcc341001483830002ad4dc9cd7c2c2653c3782a0ded10805e7bedca493a16ebe5be16b244299b94215bcb19879fab1936d8c22084609d60429727040c06ed0005d73f78410014838400029d6d34caaae5e01b5f135371ed10805e7bedca493a16ebe5be16b244299bdd1c630645aac25cd59530c6201f2b5060429735040bea2d00035a6b8001000100";

}

static altintegration::PopData generateRandPopData()
{
    // add PopData
    auto atv = altintegration::AssertDeserializeFromHex<altintegration::ATV>(VeriBlockTest::defaultAtvEncoded);
    auto vtb = altintegration::AssertDeserializeFromHex<altintegration::VTB>(VeriBlockTest::defaultVtbEncoded);


    altintegration::PopData popData;
    popData.atvs = {atv};
    popData.vtbs = {vtb, vtb, vtb};

    return popData;
}

BOOST_AUTO_TEST_CASE(GetBlockWeight_test)
{
    // Create random block
    CBlock block;
    block.hashMerkleRoot.SetNull();
    block.hashPrevBlock.SetNull();
    block.nBits = 10000;
    block.nNonce = 10000;
    block.nTime = 10000;
    block.nVersion = 1 | VeriBlock::POP_BLOCK_VERSION_BIT;

    int64_t expected_block_weight = GetBlockWeight(block);

    BOOST_CHECK(expected_block_weight > 0);

    altintegration::PopData popData = generateRandPopData();

    int64_t popDataWeight = VeriBlock::GetPopDataWeight(popData);

    BOOST_CHECK(popDataWeight > 0);

    // put PopData into block
    block.popData = popData;

    int64_t new_block_weight = GetBlockWeight(block);
    BOOST_CHECK_EQUAL(new_block_weight, expected_block_weight);
}

BOOST_AUTO_TEST_CASE(block_serialization_test)
{
    // Create random block
    CBlock block;
    block.hashMerkleRoot.SetNull();
    block.hashPrevBlock.SetNull();
    block.nBits = 10000;
    block.nNonce = 10000;
    block.nTime = 10000;
    block.nVersion = 1 | VeriBlock::POP_BLOCK_VERSION_BIT;

    altintegration::PopData popData = generateRandPopData();

    block.popData = popData;

    CDataStream stream(SER_NETWORK, PROTOCOL_VERSION);
    BOOST_CHECK(stream.size() == 0);
    stream << block;
    BOOST_CHECK(stream.size() != 0);

    CBlock decoded_block;
    stream >> decoded_block;

    BOOST_CHECK(decoded_block.GetHash() == block.GetHash());
    BOOST_CHECK(decoded_block.popData == block.popData);
}

BOOST_AUTO_TEST_CASE(block_network_passing_test)
{
    // Create random block
    CBlock block;
    block.hashMerkleRoot.SetNull();
    block.hashPrevBlock.SetNull();
    block.nBits = 10000;
    block.nNonce = 10000;
    block.nTime = 10000;
    block.nVersion = 1 | VeriBlock::POP_BLOCK_VERSION_BIT;

    altintegration::PopData popData = generateRandPopData();

    block.popData = popData;

    CDataStream stream(SER_NETWORK, PROTOCOL_VERSION);
}

BOOST_FIXTURE_TEST_CASE(BlockPoPVersion_test, E2eFixture)
{
    for (size_t i = 0; i < 400; ++i) {
        CreateAndProcessBlock({}, ChainActive().Tip()->GetBlockHash(), cbKey);
    }

    auto block = CreateAndProcessBlock({}, ChainActive().Tip()->GetBlockHash(), cbKey);
}

BOOST_AUTO_TEST_SUITE_END()
